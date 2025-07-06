#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/Type.cpp>
#include <utils/TGBotApi/JSONKeys.cpp>
#include <fmt/core.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>
#include <exception>

namespace Utils {
namespace TGBotApi {

using namespace Type;
using namespace JSONKeys;
using namespace std;
using namespace fmt;
using namespace httplib;
using namespace nlohmann;

enum EnumQueryMethod {
    GET,
    POST
};

class Query {
    private: 
    const_string _token;
    Client _https_client;

    string _get_path(const_string& path) {
        return "/bot" + _token + "/" + path;
    }

    public:

    template<typename ResultType>
    struct QueryResult {
        bool ok;
        shared_ptr<ResultType> result;
    };

    Query(const_string& token): _https_client("https://api.telegram.org"), _token(token) {}

    string query(
        const EnumQueryMethod& method,
            const_string& path, 
            const_map_string_string& params = const_map_string_string(),
            const_map_string_string& headers = const_map_string_string()
    ) {
        Params http_params; http_params.insert(params.begin(), params.end());
        Headers http_headers; http_headers.insert(headers.begin(), headers.end());
        Result result;
        
        if (method == EnumQueryMethod::GET) {
            result = _https_client.Get(_get_path(path), http_params, http_headers);
        }

        if (result.error() != Error::Success) {
            throw to_string(result.error());
        }

        return result->body;
    }

    template<typename ResultType>
    auto query(
            const EnumQueryMethod& method,
            const_string& path, 
            const_map_string_string& params = const_map_string_string(),
            const_map_string_string& headers = const_map_string_string()
        ) {

        auto json_result = json::parse(query(method, path, params, headers));

        return QueryResult<ResultType>{
            .ok = json_result[OK_KEY],
            .result = make_shared<ResultType>(json_result[RESULT_KEY])
        };
    }
};

}
}