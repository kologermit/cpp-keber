#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/Type.cpp>
#include <fmt/core.h>
#include <httplib.h>
#include <string>
#include <exception>

namespace Utils {
namespace TGBotApi {

using namespace Type;
using namespace std;
using namespace httplib;
using namespace fmt;

const_string RESULT_KEY = "result";
const_string ID_KEY = "id";
const_string FIRST_NAME_KEY = "first_name";
const_string USERNAME_KEY = "username";

class Query {
    private: 
    const_string _token;
    Client _https_client;

    string _get_path(const_string& path) {
        return "/bot" + _token + "/" + path;
    }

    public:

    Query(const_string& token): _https_client("https://api.telegram.org"), _token(token) {}

    auto Get(
            const_string& path, 
            const_map_string_string& params = const_map_string_string(),
            const_map_string_string& headers = const_map_string_string()
        ) {

        Params http_params; http_params.insert(params.begin(), params.end());
        Headers http_headers; http_headers.insert(headers.begin(), headers.end());

        auto result = _https_client.Get(_get_path(path), http_params, http_headers);
        if (result.error() != Error::Success) {
            throw to_string(result.error());
        }
        return result;
    }
};

}
}