#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>
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
        DELETE
    };

    enum ParamType {INT, FLOAT, STRING, BOOL};

    struct Param {
        string name;
        ParamType type;
    };

    struct HandlerSignature {
        string name;
        string pattern;
        RequestHandlerMethod method = RequestHandlerMethod::GET;
        bool is_auth = true;
        vector<Param> query_params = vector<Param>();
        vector<Param> headers = vector<Param>();
        vector<Param> body_params = vector<Param>();
    };

    template <typename HandlerContext>
    struct InterfaceHandler {
        virtual ~InterfaceHandler() = default;

        [[nodiscard]] virtual const HandlerSignature& get_signature() const noexcept = 0;
        [[nodiscard]] virtual json handle(shared_ptr<HandlerContext> context) = 0;
    };

}