#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <map>
#include <string>
#include <memory>
#include <vector>

namespace Utils::HTTPServer::Handler {

    using std::string;
    using std::shared_ptr;
    using std::vector;
    using httplib::Request;
    using httplib::Response;
    using nlohmann::json;

    enum RequestHandlerMethod {
        GET,
        POST,
        PATCH,
        DELETE,
        NONE
    };

    enum ParamType {INT, FLOAT, STRING, BOOL, OBJECT, ARRAY};

    struct Param {
        string name;
        ParamType type;
        bool is_required = true;
    };

    struct HandlerSignature {
        string name;
        string pattern;
        RequestHandlerMethod method = RequestHandlerMethod::GET;
        bool is_auth = false;
        bool is_json_body = false;
        vector<Param> query_params = vector<Param>();
        vector<Param> headers = vector<Param>();
        vector<Param> body_params = vector<Param>();
    };

    template <typename HandlerContext>
    struct InterfaceHandler {
        using ptrContext = shared_ptr<HandlerContext>;
        virtual ~InterfaceHandler() = default;

        [[nodiscard]] virtual const HandlerSignature& get_signature() const noexcept = 0;
        [[nodiscard]] virtual json handle(ptrContext context) = 0;
    };
}

namespace std {
    using Utils::HTTPServer::Handler::RequestHandlerMethod;
    const string& to_string(RequestHandlerMethod method) {
        static const map<RequestHandlerMethod, string> m{
            {RequestHandlerMethod::GET, "GET"},
            {RequestHandlerMethod::POST, "POST"},
            {RequestHandlerMethod::PATCH, "PATCH"},
            {RequestHandlerMethod::DELETE, "DELETE"},
            {RequestHandlerMethod::NONE, "NONE"},
        };
        return m.at(method);
    }
}