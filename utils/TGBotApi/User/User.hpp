#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

namespace Utils::TGBotApi::User {

using std::nullopt;
using std::string_view;
using std::string;
using nlohmann::json;

struct User {
    User(
        long long id,
        string_view name, 
        string_view username = ""
    );
    User(const json&);

    long long id;
    string name;
    string username;
};
}
