#pragma once

#include <string_view>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>

namespace Utils::TGBotApi::User {

using std::nullopt;
using std::string_view;
using nlohmann::json;

struct User : virtual InterfaceUser {
    explicit User(
        long long id,
        string_view name, 
        optional_const_string username = nullopt
    );
    explicit User(const json&);

    long long get_id() const noexcept override;
    const_string get_name() const noexcept override;
    optional_const_string get_username() const noexcept override;

    
    private:


    long long _id;
    const_string _name;
    optional_const_string _username;
};
}
