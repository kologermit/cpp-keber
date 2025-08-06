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
id(id), 
name(name), 
username(username)
{}

User::User(const json& json_user):
id(json_user[ID_KEY]),
name(string(json_user[FIRST_NAME_KEY]) + (
    json_user.contains(LAST_NAME_KEY) 
    ? " " + string(json_user[LAST_NAME_KEY])
    : ""
)),
username(
    (!json_user.contains(USERNAME_KEY))
    ? ""
    : json_user[USERNAME_KEY].is_null() 
        ? ""
        : json_user[USERNAME_KEY]
) {}

}
