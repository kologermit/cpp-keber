#pragma once

#include <string_view>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>

namespace Utils::TGBotApi::User {

using std::string_view;
using nlohmann::json;

struct User : virtual InterfaceUser {
    explicit User(
        int id,
        string_view name, 
        string_view username
    );
    explicit User(const json&);

    int get_id() const noexcept override;
    const_string get_name() const noexcept override;
    const_string get_username() const noexcept override;

    
    protected:


    int _id;
    const_string _name;
    const_string _username;
};
}
