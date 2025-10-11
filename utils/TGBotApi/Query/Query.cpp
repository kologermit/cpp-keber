#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/TGBotApi/Query/Query.hpp>
#include <fmt/core.h>
#include <fstream>
#include <stdexcept>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::TGBotApi::Query {

    using std::ios;
    using std::ifstream;
    using std::streamsize;
    using std::to_string;
    using std::runtime_error;
    using fmt::format;
    using httplib::Result, httplib::Error;
    using httplib::MultipartFormDataItems;
    using httplib::MultipartFormData;

    #ifndef NDEBUG
    using Utils::Logger::get_logger;
    #endif

    Query::Query(string_view token): 
    _token(token.data()) {}

    string Query::_read_file(string_view filename) {
        ifstream file(string(filename), ios::binary | ios::ate);
        streamsize size = file.tellg();
        file.seekg(0, ios::beg);
        string buffer(static_cast<size_t>(size), '\0');
        file.read(buffer.data(), size);
        return buffer;
    }

    Result Query::query(
        EnumQueryMethod method,
        string_view path, 
        const Params& params,
        const Headers& headers,
        const Files& files,
        string_view full_path
    ) {
        Result result;
        string result_path = (
            full_path.empty()
            ? fmt::format("/bot{}/{}", _token, path)
            : fmt::format(fmt::runtime(full_path), _token)
        );

        #ifndef NDEBUG
        get_logger()->debug("Method", (
            method == EnumQueryMethod::GET ? "GET"
            : method == EnumQueryMethod::POST ? "POST"
            : "UNKNOWN"
        ), __FILE__, __LINE__);
        get_logger()->debug("Path", path, __FILE__, __LINE__);
        get_logger()->debug("Result_path", result_path, __FILE__, __LINE__);
        get_logger()->debug("Params_is_empty", to_string(params.empty()), __FILE__, __LINE__);
        for (const auto&[param, value] : params) {
            get_logger()->debug("Params", param + " : " + value, __FILE__, __LINE__);
        }
        get_logger()->debug("Headers_is_empty", to_string(headers.empty()), __FILE__, __LINE__);
        for (const auto&[header, value] : headers) {
            get_logger()->debug("Headers", header + " : " + value, __FILE__, __LINE__);
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
        cli.enable_server_certificate_verification(false);
        
        if (method == EnumQueryMethod::GET) {
            result = cli.Get(result_path, params, headers);
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
            result = cli.Post(result_path, headers, form_data);
        }

        if (result.error() != Error::Success) {
            throw runtime_error(fmt::format("Failed query: {}", to_string(result.error())));
        }

        #ifndef NDEBUG
        if (!result->body.empty() && result->body[0] == '{') {
            get_logger()->debug("Body", result->body, __FILE__, __LINE__);
        }
        #endif

        return result;
    }
}
