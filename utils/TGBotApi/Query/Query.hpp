#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/TGBotApi/JSONKeys.hpp>
#include <httplib.h>
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <stdexcept>
#include <vector>

namespace Utils::TGBotApi::Query {

    using httplib::Params;
    using httplib::Headers;
    using httplib::Result;
    using nlohmann::json;
    using std::vector;
    using std::exception;
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

    enum QueryMethod {
        GET,
        POST
    };

    struct Query {
        explicit Query(string_view token, string_view telegram_api_url = "https://api.telegram.org");

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

        Result query(
            QueryMethod method,
            string_view path, 
            const Params& params = {},
            const Headers& headers = {},
            const Files& files = {},
            string_view full_path = "",
            bool throw_by_status = false
        );

        inline json query_raw_json(
            QueryMethod method,
            string_view path,
            const Params& params = {},
            const Headers& headers = {},
            const Files& files = {},
            string_view full_path = "",
            bool throw_by_status = false
        );

        template<typename ResultType>
        QueryResult<ResultType> query_parse_json(
            QueryMethod method,
            string_view path,
            const Params& params = {},
            const Headers& headers = {},
            const Files& files = {},
            string_view full_path = "",
            bool throw_by_status = false
        );

        private: 
        const string _token;
        const string _telegram_api_url;

        static string _read_file(string_view filename);
    };

    inline json Query::query_raw_json(
        QueryMethod method,
        string_view path, 
        const Params& params,
        const Headers& headers,
        const Files& files,
        string_view full_path,
        bool throw_by_status
    ) {
        return json::parse(query(method, path, params, headers, files, full_path, throw_by_status)->body);
    }


    template<typename ResultType>
    Query::QueryResult<ResultType> Query::query_parse_json(
        QueryMethod method,
        string_view path, 
        const Params& params,
        const Headers& headers,
        const Files& files,
        string_view full_path,
        bool throw_by_status
    ) {
        json json_result = query_raw_json(method, path, params, headers, files, full_path, throw_by_status);

        if (!static_cast<bool>(json_result[OK_KEY])) {
            throw runtime_error(format(
                "Utils::TGBotApi::Query::query_parse_json: {} -- {}",
                json_result[ERROR_CODE_KEY].get<int>(),
                json_result[DESCRIPTION_KEY].get<string>()
            ));
        }

        return {
            json_result[OK_KEY],
            make_unique<ResultType>(json_result[RESULT_KEY])
        };
    }
}
