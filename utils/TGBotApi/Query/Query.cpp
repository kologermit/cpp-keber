#define CPPHTTPLIB_OPENSSL_SUPPORT

#ifdef DEBUG_LOGGER
#include <utils/Logger/InterfaceLogger.hpp>
#endif


#include <utils/TGBotApi/Query/Query.hpp>
#include <fmt/core.h>
#include <fstream>
#include <sstream>

namespace Utils::TGBotApi::Query {

using std::ifstream;
using std::ios;
using std::vector;
using std::streamsize;
using std::to_string;
using std::cout, std::endl;
using httplib::Result;
using httplib::Error;
using httplib::MultipartFormDataItems;

#ifdef DEBUG_LOGGER
using Utils::Logger::get_logger;
#endif

Query::Query(string_view token): 
_token(token) {}

string Query::_get_path(string_view path) {
    return "/bot" + _token + "/" + const_string(path);
}

string Query::_read_file(string_view filename) {
    ifstream file(filename.data(), ios::binary | ios::ate);
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

    httplib::Client cli("https://api.telegram.org");
    
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
            full_path,
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


}
