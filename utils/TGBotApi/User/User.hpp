#pragma once

#include <string_view>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>

namespace Utils::TGBotApi::User {

using std::string_view;
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
}
