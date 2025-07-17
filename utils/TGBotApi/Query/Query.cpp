#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/TGBotApi/Query/Query.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/Logger/InterfaceLogger.hpp>
#include <fmt/core.h>
#include <fstream>
#include <sstream>

namespace Utils::TGBotApi::Query {

using std::runtime_error;
using std::ifstream;
using std::ios;
using std::vector;
using std::streamsize;
using std::make_shared;
using std::to_string;
using std::cout, std::endl;
using fmt::format;
using httplib::Result;
using httplib::Error;
using httplib::MultipartFormDataItems;
using Utils::TGBotApi::JSONKeys::RESULT_KEY;
using Utils::TGBotApi::JSONKeys::OK_KEY;
using Utils::TGBotApi::JSONKeys::DESCRIPTION_KEY;
using Utils::TGBotApi::JSONKeys::ERROR_CODE_KEY;
using Utils::Logger::get_logger;

Query::Query(string_view token): 
_token(token) {}

string Query::_get_path(string_view path) {
    return "/bot" + _token + "/" + const_string(path);
}

string Query::_read_file(string_view filename) {
    ifstream file(string(filename), ios::binary | ios::ate);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);
    vector<char> buffer(size);
    file.read(buffer.data(), size);
    return string(buffer.begin(), buffer.end());
}

const_string Query::query(
    const EnumQueryMethod& method,
    string_view path, 
    OptionalParams params,
    OptionalHeaders headers,
    OptionalFiles files
) {
    Result result;
    string full_path = _get_path(path);

    #ifdef DEBUG_LOGGER

    get_logger()->debug("Query::query::method", (
        method == EnumQueryMethod::GET ? "GET"
        : method == EnumQueryMethod::POST ? "POST"
        : "UNKNOWN"
    ));
    get_logger()->debug("Query::query::path", path);
    get_logger()->debug("Query::query::full_path", full_path);
    get_logger()->debug("Query::query::params_is_empty", to_string(params.value_or(Params()).empty()));
    for (auto param : params.value_or(Params())) {
        get_logger()->debug("Query::query::params", param.first + " : " + param.second);
    }
    get_logger()->debug("Query::query::headers_is_empty", to_string(headers.value_or(Headers()).empty()));
    for (auto header : headers.value_or(Headers())) {
        get_logger()->debug("Query::query::headers", header.first + " : " + header.second);
    }
    get_logger()->debug("Query::query::files::is_empty", to_string(files.value_or(Files{}).empty()));
    for (auto file : files.value_or(Files{})) {
        get_logger()->debug("Query::query::files", format(
            "name={}; filename={}; filepath={}; content_type={}",
            file.name,
            file.filename,
            file.filepath,
            file.content_type
        ));
    }


    #endif

    Client cli("https://api.telegram.org");
    
    if (method == EnumQueryMethod::GET) {
        result = cli.Get(
            full_path,
            params.value_or(Params()), 
            headers.value_or(Headers())
        );
    } else if (method == EnumQueryMethod::POST) {
        MultipartFormDataItems form_data;
        if (params.has_value()) {
            for (auto param : *params) {
                form_data.push_back({
                    .name = param.first,
                    .content = param.second
                });
            }  
        }
        if (files.has_value()) {
            for (auto file : *files) {
                form_data.push_back({
                    .name = file.name,
                    .content = _read_file(file.filepath),
                    .filename = file.filename,
                    .content_type = file.content_type
                });
            }
        }
        result = cli.Post(
            _get_path(path),
            headers.value_or(Headers()),
            form_data  
        );
    }

    if (result.error() != Error::Success) {
        throw runtime_error(to_string(result.error()));
    }

    #ifdef DEBUG_LOGGER

    get_logger()->debug("Query::query::result->body", result->body);

    #endif

    return result->body;
}

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
