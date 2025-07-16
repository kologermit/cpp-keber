#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/Types.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <optional>
#include <memory>
#include <vector>

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
using httplib::Client;

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
        ResultType result;
    };

    const_string query(
        const EnumQueryMethod& method,
        std::string_view path, 
        OptionalParams params = nullopt,
        OptionalHeaders headers = nullopt,
        OptionalFiles files = nullopt
    );

    const json query_raw_json(
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
    Client _https_client;
    string _get_path(std::string_view path);
    string _read_file(std::string_view filename);
};

}
