#include <utils/TGBotApi/User/User.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

namespace Utils::TGBotApi::User {

using Utils::TGBotApi::JSONKeys::ID_KEY;
using Utils::TGBotApi::JSONKeys::FIRST_NAME_KEY;
using Utils::TGBotApi::JSONKeys::USERNAME_KEY;

User::User(
    int id,
    string_view name, 
    optional_string_view username
    ): 
    _id(id), 
    _name(name), 
    _username(username)
    {}

User::User(const json& json_user):
_id(json_user[ID_KEY]),
_name(json_user[FIRST_NAME_KEY]),
_username(
    (!json_user.contains(USERNAME_KEY))
    ? nullopt
    : json_user[USERNAME_KEY].is_null() 
        ? nullopt
        : optional_string_view(to_string(json_user[USERNAME_KEY]))
) {}

int User::get_id() const noexcept { return _id; }
const_string User::get_name() const noexcept { return _name; }
optional_const_string User::get_username() const noexcept { return _username; }

}
