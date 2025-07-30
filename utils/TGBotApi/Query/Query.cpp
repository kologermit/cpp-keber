#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/TGBotApi/Query/Query.hpp>
#include <fmt/core.h>
#include <fstream>
#include <sstream>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::TGBotApi::Query {

using std::ios, std::ifstream, std::streamsize;
using fmt::format;
using httplib::Result, httplib::Error;
using httplib::MultipartFormDataItems;
using httplib::MultipartFormData;

#ifndef NDEBUG
using Utils::Logger::get_logger;
#endif

Query::Query(string_view token): 
_token(token) {}

string Query::_get_path(string_view path) {
    return format(
        "/bot{}/{}",
        _token,
        path
    );
}

string Query::_read_file(string_view filename) {
    ifstream file(string(filename), ios::binary | ios::ate);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);
    string buffer(size, '\0');
    file.read(buffer.data(), size);
    return buffer;
}

string Query::query(
    EnumQueryMethod method,
    string_view path, 
    const Params& params,
    const Headers& headers,
    const Files& files
) {
    Result result;
    string full_path = _get_path(path);

    #ifndef NDEBUG
    get_logger()->debug(__FILE__, __LINE__, "Query::query::method", (
        method == EnumQueryMethod::GET ? "GET"
        : method == EnumQueryMethod::POST ? "POST"
        : "UNKNOWN"
    ));
    get_logger()->debug(__FILE__, __LINE__, "Query::query::path", path);
    get_logger()->debug(__FILE__, __LINE__, "Query::query::full_path", full_path);
    get_logger()->debug(__FILE__, __LINE__, "Query::query::params_is_empty", std::to_string(params.empty()));
    for (auto param : params) {
        get_logger()->debug(__FILE__, __LINE__, "Query::query::params", param.first + " : " + param.second);
    }
    get_logger()->debug(__FILE__, __LINE__, "Query::query::headers_is_empty", std::to_string(headers.empty()));
    for (auto header : headers) {
        get_logger()->debug(__FILE__, __LINE__, "Query::query::headers", header.first + " : " + header.second);
    }
    get_logger()->debug(__FILE__, __LINE__, "Query::query::files::is_empty", std::to_string(files.empty()));
    for (auto file : files) {
        get_logger()->debug(__FILE__, __LINE__, "Query::query::files", format(
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
        result = cli.Get(full_path, params, headers);
    } else if (method == EnumQueryMethod::POST) {
        MultipartFormDataItems form_data;
        if (!params.empty()) {
            for (const auto& param : params) {
                form_data.push_back({
                    param.first,
                    param.second,
                    "",
                    "",
                });
            }  
        }
        if (!files.empty()) {
            for (const auto& file : files) {
                form_data.push_back(MultipartFormData{
                    file.name,
                    _read_file(file.filepath),
                    file.filename,
                    file.content_type
                });
            }
        }
        result = cli.Post(full_path, headers, form_data);
    }

    if (result.error() != Error::Success) {
        throw runtime_error(to_string(result.error()));
    }

    #ifndef NDEBUG
    get_logger()->debug(__FILE__, __LINE__, "Query::query::result->body", result->body);
    #endif

    return result->body;
}


}
