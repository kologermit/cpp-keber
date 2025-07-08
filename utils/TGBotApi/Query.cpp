#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/Type.cpp>
#include <utils/TGBotApi/JSONKeys.cpp>
#include <fmt/core.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <exception>

namespace Utils::TGBotApi::Query {

using Utils::Type::const_string;
using Utils::Type::const_map_string_to_string;
using Utils::TGBotApi::JSONKeys::RESULT_KEY;
using Utils::TGBotApi::JSONKeys::OK_KEY;
using nlohmann::json;
using std::string;
using std::to_string;
using std::runtime_error;
using std::string_view;
using httplib::Client;
using httplib::Params;
using httplib::Headers;
using httplib::Result;
using httplib::Error;

enum EnumQueryMethod {
    GET,
    POST
};

struct Query {
    Query(string_view token): _https_client("https://api.telegram.org"), _token(token) {}

    template<typename ResultType>
    struct QueryResult {
        bool ok;
        ResultType result;
    };

    const_string query(
        const EnumQueryMethod& method,
        string_view path, 
        const_map_string_to_string& params = {},
        const_map_string_to_string& headers = {}
    );

    const json query_raw_json(
        const EnumQueryMethod& method,
        string_view path,
        const_map_string_to_string& params = {},
        const_map_string_to_string& headers = {}
    );

    template<typename ResultType>
    QueryResult<ResultType> query_parse_json(
        const EnumQueryMethod& method,
        string_view path, 
        const_map_string_to_string& params = {},
        const_map_string_to_string& headers = {}
    );

    protected: 
    const_string _token;
    Client _https_client;
    string _get_path(string_view path) {
        return "/bot" + _token + "/" + const_string(path);
    }
};

const_string Query::query(
    const EnumQueryMethod& method,
    string_view path, 
    const_map_string_to_string& params,
    const_map_string_to_string& headers
) {
    Params http_params; http_params.insert(params.begin(), params.end());
    Headers http_headers; http_headers.insert(headers.begin(), headers.end());
    Result result;
    
    if (method == EnumQueryMethod::GET) {
        result = _https_client.Get(_get_path(path), http_params, http_headers);
    }

    if (result.error() != Error::Success) {
        throw runtime_error(to_string(result.error()));
    }

    return result->body;
}

const json Query::query_raw_json(
    const EnumQueryMethod& method,
    string_view path, 
    const_map_string_to_string& params,
    const_map_string_to_string& headers
) {
    return json::parse(query(method, path, params, headers));
}

template<typename ResultType>
Query::QueryResult<ResultType> Query::query_parse_json(
    const EnumQueryMethod& method,
    string_view path, 
    const_map_string_to_string& params,
    const_map_string_to_string& headers
) {
    auto json_result = query_raw_json(method, path, params, headers);

    return {
        json_result[OK_KEY],
        ResultType(json_result[RESULT_KEY])
    };
}

}