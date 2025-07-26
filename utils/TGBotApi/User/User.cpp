#include <utils/TGBotApi/User/User.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

namespace Utils::TGBotApi::User {

using Utils::TGBotApi::JSONKeys::ID_KEY;
using Utils::TGBotApi::JSONKeys::FIRST_NAME_KEY;
using Utils::TGBotApi::JSONKeys::LAST_NAME_KEY;
using Utils::TGBotApi::JSONKeys::USERNAME_KEY;

User::User(
    long long id,
    string_view name, 
    string_view username
    ): 
    _id(id), 
    _name(name), 
    _username(username)
    {}

User::User(const json& json_user):
_id(json_user[ID_KEY]),
_name(string(json_user[FIRST_NAME_KEY]) + (
    json_user.contains(LAST_NAME_KEY) 
    ? " " + string(json_user[LAST_NAME_KEY])
    : ""
)),
_username(
    (!json_user.contains(USERNAME_KEY))
    ? ""
    : json_user[USERNAME_KEY].is_null() 
        ? ""
        : json_user[USERNAME_KEY]
) {}

long long User::get_id() const noexcept { return _id; }
string User::get_name() const noexcept { return _name; }
string User::get_username() const noexcept { return _username; }

}
