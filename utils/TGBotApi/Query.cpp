#include <utils/TGBotApi/Query.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <fstream>
#include <sstream>

namespace Utils::TGBotApi::Query {

using std::runtime_error;
using std::ifstream;
using std::ios;
using std::vector;
using std::streamsize;
using std::make_shared;
using httplib::Result;
using httplib::Error;
using httplib::MultipartFormDataItems;
using Utils::TGBotApi::JSONKeys::RESULT_KEY;
using Utils::TGBotApi::JSONKeys::OK_KEY;

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
    
    if (method == EnumQueryMethod::GET) {
        result = _https_client.Get(
            _get_path(path), 
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
        result = _https_client.Post(
            _get_path(path),
            headers.value_or(Headers()),
            form_data  
        );
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
