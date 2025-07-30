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
using std::to_string;
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
    get_logger()->debug("Method", (
        method == EnumQueryMethod::GET ? "GET"
        : method == EnumQueryMethod::POST ? "POST"
        : "UNKNOWN"
    ), __FILE__, __LINE__);
    get_logger()->debug("Path", path, __FILE__, __LINE__);
    get_logger()->debug("Full_path", full_path, __FILE__, __LINE__);
    get_logger()->debug("Params_is_empty", to_string(params.empty()), __FILE__, __LINE__);
    for (auto param : params) {
        get_logger()->debug("Params", param.first + " : " + param.second, __FILE__, __LINE__);
    }
    get_logger()->debug("Headers_is_empty", to_string(headers.empty()), __FILE__, __LINE__);
    for (auto header : headers) {
        get_logger()->debug("Headers", header.first + " : " + header.second, __FILE__, __LINE__);
    }
    get_logger()->debug("Files_is_empty", to_string(files.empty()), __FILE__, __LINE__);
    for (auto file : files) {
        get_logger()->debug("Files", format(
            "name={}; filename={}; filepath={}; content_type={}",
            file.name,
            file.filename,
            file.filepath,
            file.content_type
        ), __FILE__, __LINE__);
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
    get_logger()->debug("Result->body", result->body, __FILE__, __LINE__);
    #endif

    return result->body;
}


}
