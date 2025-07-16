#pragma once

#include <string_view>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>

namespace Utils::TGBotApi::User {

using Utils::Types::optional_string_view;
using std::nullopt;
using std::string_view;
using nlohmann::json;

struct User : virtual InterfaceUser {
    explicit User(
        int id,
        string_view name, 
        optional_string_view username = nullopt
    );
    explicit User(const json&);

    int get_id() const noexcept override;
    const_string get_name() const noexcept override;
    optional_const_string get_username() const noexcept override;

    
    private:


    int _id;
    const_string _name;
    optional_const_string _username;
};
}
