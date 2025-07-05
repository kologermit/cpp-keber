#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <string>
#include <fmt/core.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <utils/Logger/InterfaceLogger.cpp>
#include <utils/Type.cpp>
#include <utils/UUID4/UUID4.cpp>
#include <utils/TGBotApi/Query.cpp>

namespace Utils {
namespace TGBotApi {

using namespace std;
using namespace fmt;
using namespace Utils;
using namespace Utils::Type;
using namespace Utils::Logger;
using namespace httplib;
using namespace nlohmann;

class Bot {

    private:
    const_string _token;
    const_string _webhook_url;
    const_string _secret;
    string _name;
    string _username;
    int _user_id;

    auto getMe() const {
        return Query(_token).Get("getMe");
    }

    public:
    Bot(
        const_string& token, 
        const_string& webhook_url
    ):
    _token(token),
    _webhook_url(webhook_url),
    _secret(UUID4::generate_str())
    {
        Result result = getMe();
        json json_me = json::parse(result->body);
        _user_id = json_me[RESULT_KEY][ID_KEY];
        _name = json_me[RESULT_KEY][FIRST_NAME_KEY];
        _username = json_me[RESULT_KEY][USERNAME_KEY];
    }

    int get_id() const {
        return _user_id;
    }

    string get_name() const {
        return _name;
    }

    string get_username() const {
        return _username;
    }
};

}
}