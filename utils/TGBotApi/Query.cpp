#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/Type.cpp>
#include <utils/TGBotApi/JSONKeys.cpp>
#include <fmt/core.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
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

template<typename ResultType> 
class Query {
    private: 
    const_string _token;
    Client _https_client;

    string _get_path(const_string& path) {
        return "/bot" + _token + "/" + path;
    }

    public:

    struct QueryResult {
        bool ok;
        ResultType result;
    };

    Query(const_string& token): _https_client("https://api.telegram.org"), _token(token) {}

    auto Get(
            const_string& path, 
            const_map_string_string& params = const_map_string_string(),
            const_map_string_string& headers = const_map_string_string()
        ) {

        Params http_params; http_params.insert(params.begin(), params.end());
        Headers http_headers; http_headers.insert(headers.begin(), headers.end());
        Result result = _https_client.Get(_get_path(path), http_params, http_headers);

        if (result.error() != Error::Success) {
            throw to_string(result.error());
        }

        auto json_result = json::parse(result->body);

        return QueryResult{
            bool(json_result[OK_KEY]),
            ResultType(json_result[RESULT_KEY])
        };
    }
};

}
}