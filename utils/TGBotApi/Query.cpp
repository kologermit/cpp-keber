#include <utils/TGBotApi/Query.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

namespace Utils::TGBotApi::Query {

using std::runtime_error;
using httplib::Params;
using httplib::Headers;
using httplib::Result;
using httplib::Error;
using Utils::TGBotApi::JSONKeys::RESULT_KEY;
using Utils::TGBotApi::JSONKeys::OK_KEY;

Query::Query(string_view token): _https_client("https://api.telegram.org"), _token(token) {}

string Query::_get_path(string_view path) {
    return "/bot" + _token + "/" + const_string(path);
}

const_string Query::query(
    const EnumQueryMethod& method,
    string_view path, 
    const_map_string_to_string& params,
    const_map_string_to_string& headers
) {
    Params http_params; http_params.insert(params.begin(), params.end());
    Headers http_headers; http_headers.insert(headers.begin(), headers.end());
    Result result;
    
    if (method == EnumQueryMethod::GET) {
        result = _https_client.Get(_get_path(path), http_params, http_headers);
    }

    if (result.error() != Error::Success) {
        throw runtime_error(to_string(result.error()));
    }

    return result->body;
}

const json Query::query_raw_json(
    const EnumQueryMethod& method,
    string_view path, 
    const_map_string_to_string& params,
    const_map_string_to_string& headers
) {
    return json::parse(query(method, path, params, headers));
}

template<typename ResultType>
Query::QueryResult<ResultType> Query::query_parse_json(
    const EnumQueryMethod& method,
    string_view path, 
    const_map_string_to_string& params,
    const_map_string_to_string& headers
) {
    auto json_result = query_raw_json(method, path, params, headers);

    return {
        json_result[OK_KEY],
        ResultType(json_result[RESULT_KEY])
    };
}

}
