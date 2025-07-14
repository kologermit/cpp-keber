#include <utils/TGBotApi/Query.hpp>
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
using Utils::Logger::get_logger;

Query::Query(string_view token): _https_client("https://api.telegram.org"), _token(token) {}

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

    // #define DEBUG_LOGGER

    // #ifdef DEBUG_LOGGER

    // get_logger()->debug("Query::query::method", to_string(method));
    // get_logger()->debug("Query::query::path", path);
    // get_logger()->debug("Query::query::full_path", full_path);
    // get_logger()->debug("Query::query::params_is_empty", to_string(params.value_or(Params()).empty()));
    // for (auto param : params.value_or(Params())) {
    //     get_logger()->debug("Query::query::params", param.first + " : " + param.second);
    // }
    // get_logger()->debug("Query::query::headers_is_empty", to_string(headers.value_or(Headers()).empty()));
    // for (auto header : headers.value_or(Headers())) {
    //     get_logger()->debug("Query::query::headers", header.first + " : " + header.second);
    // }
    // get_logger()->debug("Query::query::files::is_empty", to_string(files.value_or(Files{}).empty()));
    // for (auto file : files.value_or(Files{})) {
    //     get_logger()->debug("Query::query::files", format(
    //         "name={}; filename={}; filepath={}; content_type={}",
    //         file.name,
    //         file.filename,
    //         file.filepath,
    //         file.content_type
    //     ));
    // }


    // #endif
    
    if (method == EnumQueryMethod::GET) {
        get_logger()->debug("Method", "1");
        auto result = Client("https://api.telegram.org").Get(
            "/bot7699806249:AAE7JYJ9oV4DLQ4QRRIOhesHiIwvOX-hBk0/getMe", 
            Params(), 
            Headers()
        );
        cout << result.error() << endl;
        cout << result->body << endl;
        get_logger()->debug("Query::error", to_string(result.error()));
        get_logger()->debug("Query::result", result->body);
    } else if (method == EnumQueryMethod::POST) {
        get_logger()->debug("Method", "2");
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
        result = _https_client.Post(
            _get_path(path),
            headers.value_or(Headers()),
            form_data  
        );
    } else {
        get_logger()->debug("Method", "else");
    }

    if (result.error() != Error::Success) {
        throw runtime_error(to_string(result.error()));
    }

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

    return {
        json_result[OK_KEY],
        ResultType(json_result[RESULT_KEY])
    };
}

}
