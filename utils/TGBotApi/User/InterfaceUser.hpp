#pragma once

#include <utils/Types.hpp>

namespace Utils::TGBotApi::User {

using Utils::Types::const_string;
using Utils::Types::optional_const_string;

struct InterfaceUser {
    virtual long long get_id() const noexcept = 0;
    virtual const_string get_name() const noexcept = 0;
    virtual optional_const_string get_username() const noexcept = 0;

    ~InterfaceUser() = default;
};

}
