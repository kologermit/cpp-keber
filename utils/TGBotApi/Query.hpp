#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/Types.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

namespace Utils::TGBotApi::Query {

using Utils::Types::const_string;
using Utils::Types::const_map_string_to_string;
using nlohmann::json;
using std::string;
using std::string_view;
using httplib::Client;

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
    string _get_path(string_view path);
};

}
