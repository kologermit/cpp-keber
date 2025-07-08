#pragma once

#include <utils/Type.cpp>

namespace Utils::TGBotApi::User {

using Utils::Type::const_string;

struct InterfaceUser {
    virtual int get_id() const noexcept = 0;
    virtual const_string get_name() const noexcept = 0;
    virtual const_string get_username() const noexcept = 0;

    ~InterfaceUser() = default;
};

}