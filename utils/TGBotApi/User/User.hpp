#pragma once

#include <utils/TGBotApi/User/InterfaceUser.hpp>
#include <nlohmann/json.hpp>
#include <string_view>

namespace Utils::TGBotApi::User {

using std::nullopt;
using std::string_view;
using nlohmann::json;

struct User : virtual InterfaceUser {
    User(
        long long id,
        string_view name, 
        string_view username = ""
    );
    User(const json&);

    long long get_id() const noexcept override;
    string get_name() const noexcept override;
    string get_username() const noexcept override;

    
    private:


    long long _id;
    string _name;
    string _username;
};
}
