#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/Types.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <optional>
#include <memory>
#include <vector>
#include <fmt/core.h>

namespace Utils::TGBotApi::Query {

using Utils::Types::const_string;
using Utils::Types::const_multimap_string_to_string;
using Utils::Types::Triplet;
using httplib::Params;
using httplib::Headers;
using nlohmann::json;
using std::vector;
using std::shared_ptr;
using std::string, std::string_view;
using std::optional, std::nullopt;
using std::runtime_error;
using httplib::Client;
using fmt::format;
using std::make_shared;
using Utils::TGBotApi::JSONKeys::RESULT_KEY;
using Utils::TGBotApi::JSONKeys::OK_KEY;
using Utils::TGBotApi::JSONKeys::DESCRIPTION_KEY;
using Utils::TGBotApi::JSONKeys::ERROR_CODE_KEY;

enum EnumQueryMethod {
    GET,
    POST
};

struct Query {
    explicit Query(std::string_view token);

    struct File {
        const_string name;
        const_string filepath;
        const_string filename;
        const_string content_type;
    };

    using OptionalParams = const optional<Params>;
    using OptionalHeaders = const optional<Headers>;
    using Files = vector<File>;
    using OptionalFiles = const optional<Files>;

    template<typename ResultType>
    struct QueryResult {
        bool ok;
        std::shared_ptr<ResultType> result;
    };

    const_string query(
        const EnumQueryMethod& method,
        std::string_view path, 
        OptionalParams params = nullopt,
        OptionalHeaders headers = nullopt,
        OptionalFiles files = nullopt
    );

    inline const json query_raw_json(
        const EnumQueryMethod& method,
        std::string_view path,
        OptionalParams params = nullopt,
        OptionalHeaders headers = nullopt,
        OptionalFiles files = nullopt
    );

    template<typename ResultType>
    QueryResult<ResultType> query_parse_json(
        const EnumQueryMethod& method,
        std::string_view path,
        OptionalParams params = nullopt,
        OptionalHeaders headers = nullopt,
        OptionalFiles files = nullopt
    );

    private: 
    const_string _token;
    string _get_path(std::string_view path);
    string _read_file(std::string_view filename);
};

const json Query::query_raw_json(
    const EnumQueryMethod& method,
    string_view path, 
    OptionalParams params,
    OptionalHeaders headers,
    OptionalFiles files
) {
    return json::parse(query(method, path, params, headers, files));
}

template<typename ResultType>
Query::QueryResult<ResultType> Query::query_parse_json(
    const EnumQueryMethod& method,
    string_view path, 
    OptionalParams params,
    OptionalHeaders headers,
    OptionalFiles files
) {
    auto json_result = query_raw_json(method, path, params, headers, files);

    if (!bool(json_result[OK_KEY])) {
        throw runtime_error(format(
            "Utils::TGBotApi::Query::query_parse_json: {} -- {}",
            to_string(json_result[ERROR_CODE_KEY]),
            to_string(json_result[DESCRIPTION_KEY])
        ));
    }

    return {
        json_result[OK_KEY],
        make_shared<ResultType>(json_result[RESULT_KEY])
    };
}

}
