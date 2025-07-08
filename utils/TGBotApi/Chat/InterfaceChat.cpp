#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/User/InterfaceUser.cpp>

namespace Utils::TGBotApi::Chat {

using std::string;
using Utils::TGBotApi::User::InterfaceUser;

enum EnumChatType {
    PRIVATE
};

struct InterfaceChat : virtual InterfaceUser {

    virtual EnumChatType get_type() const noexcept = 0;

    ~InterfaceChat() = default;

};

}