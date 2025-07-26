#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/TGBotApi/JSONKeys.hpp>
#include <httplib.h>
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <vector>

namespace Utils::TGBotApi::Query {

using httplib::Params;
using httplib::Headers;
using nlohmann::json;
using std::vector;
using std::unique_ptr;
using std::string, std::string_view;
using std::optional, std::nullopt;
using std::runtime_error;
using fmt::format;
using std::make_unique;
using Utils::TGBotApi::JSONKeys::RESULT_KEY;
using Utils::TGBotApi::JSONKeys::OK_KEY;
using Utils::TGBotApi::JSONKeys::DESCRIPTION_KEY;
using Utils::TGBotApi::JSONKeys::ERROR_CODE_KEY;

enum EnumQueryMethod {
    GET,
    POST
};

struct Query {
    explicit Query(string_view token);

    struct File {
        string name;
        string filepath;
        string filename;
        string content_type;
    };

    using Files = vector<File>;

    template<typename ResultType>
    struct QueryResult {
        bool ok;
        std::unique_ptr<ResultType> result;
    };

    string query(
        EnumQueryMethod method,
        string_view path, 
        const Params& params = {},
        const Headers& headers = {},
        const Files& files = {}
    );

    inline json query_raw_json(
        EnumQueryMethod method,
        string_view path,
        const Params& params = {},
        const Headers& headers = {},
        const Files& files = {}
    );

    template<typename ResultType>
    QueryResult<ResultType> query_parse_json(
        EnumQueryMethod method,
        string_view path,
        const Params& params = {},
        const Headers& headers = {},
        const Files& files = {}
    );

    private: 
    string_view _token;
    string _get_path(string_view path);
    string _read_file(string_view filename);
};

inline json Query::query_raw_json(
    EnumQueryMethod method,
    string_view path, 
    const Params& params,
    const Headers& headers,
    const Files& files
) {
    return json::parse(query(method, path, params, headers, files));
}

template<typename ResultType>
Query::QueryResult<ResultType> Query::query_parse_json(
    EnumQueryMethod method,
    string_view path, 
    const Params& params,
    const Headers& headers,
    const Files& files
) {
    auto json_result = query_raw_json(method, path, params, headers, files);

    if (!bool(json_result[OK_KEY])) {
        throw runtime_error(format(
            "Utils::TGBotApi::Query::query_parse_json: {} -- {}",
            string(json_result[ERROR_CODE_KEY]),
            string(json_result[DESCRIPTION_KEY])
        ));
    }

    return {
        json_result[OK_KEY],
        make_unique<ResultType>(json_result[RESULT_KEY])
    };
}

}
