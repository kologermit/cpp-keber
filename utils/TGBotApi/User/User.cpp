#pragma once

#include <string>
#include <string_view>
#include <nlohmann/json.hpp>
#include <utils/Type.cpp>
#include <utils/TGBotApi/User/InterfaceUser.cpp>
#include <utils/TGBotApi/Query.cpp>
#include <utils/TGBotApi/JSONKeys.cpp>

namespace Utils::TGBotApi::User {

using std::string_view;
using Utils::Type::const_string;
using Utils::TGBotApi::JSONKeys::ID_KEY;
using Utils::TGBotApi::JSONKeys::FIRST_NAME_KEY;
using Utils::TGBotApi::JSONKeys::USERNAME_KEY;
using nlohmann::json;

struct User : virtual InterfaceUser {
    User(
        int id,
        string_view name, 
        string_view username
    );
    User(const json&);

    int get_id() const noexcept override { return _id; }
    const_string get_name() const noexcept override { return _name; }
    const_string get_username() const noexcept override { return _username; }

    
    protected:


    int _id;
    const_string _name;
    const_string _username;
};

User::User(
    int id,
    string_view name, 
    string_view username
    ): 
    _id(id), 
    _name(name), 
    _username(username)
    {}

User::User(const json& json_user):
_id(json_user[ID_KEY]),
_name(json_user[FIRST_NAME_KEY]),
_username(json_user.value(USERNAME_KEY, "")) {}

}