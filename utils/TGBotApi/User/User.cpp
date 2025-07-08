#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <utils/Type.cpp>
#include <utils/TGBotApi/User/InterfaceUser.cpp>
#include <utils/TGBotApi/Query.cpp>
#include <utils/TGBotApi/JSONKeys.cpp>

namespace Utils {
namespace TGBotApi {
namespace User {

using namespace std;
using namespace Type;
using namespace JSONKeys;
using namespace nlohmann;

struct User : virtual InterfaceUser {

    // constructors
    User(
        const_int& id=0,
        const_string& name="", 
        const_string& username=""
    );

    // interface
    int get_id() const override { return _id; }
    const_string& get_name() const override { return _name; }
    const_string& get_username() const override { return _username; }

    
    protected:

    struct UserDto {
        int id;
        const_string name;
        const_string username;

        UserDto(int id, const_string& name, const_string& username);
        UserDto(const InterfaceUser& user);
        UserDto(const json&);
    };

    User(const UserDto& user_dto);

    int _id;
    string _name;
    string _username;
};

User::User(
    const_int& id=0,
    const_string& name="", 
    const_string& username=""
    ): 
    _id(id), 
    _name(name), 
    _username(username)
    {}

User::UserDto::UserDto(
    int id = 0, 
    const_string& name = "", 
    const_string& username = ""
    ):
    id(id),
    name(name),
    username(username)
    {}

User::UserDto::UserDto(const InterfaceUser& user):
    id(user.get_id()),
    name(user.get_name()),
    username(user.get_username())
    {}

User::UserDto::UserDto(const json& json_user):
    id(json_user.value(ID_KEY, 0)),
    name(json_user.value(FIRST_NAME_KEY, "")),
    username(json_user.value(USERNAME_KEY, ""))
    {}

User::User(const UserDto& user_dto):
    _id(user_dto.id),
    _name(user_dto.name),
    _username(user_dto.username)
    {}

}
}
}