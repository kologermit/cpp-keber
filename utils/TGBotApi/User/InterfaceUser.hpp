#pragma once

#include <string>
#include <string_view>

namespace Utils::TGBotApi::User {

using std::string;

struct InterfaceUser {
    virtual long long get_id() const noexcept = 0;
    virtual string get_name() const noexcept = 0;
    virtual string get_username() const noexcept = 0;

    ~InterfaceUser() = default;
};

}
