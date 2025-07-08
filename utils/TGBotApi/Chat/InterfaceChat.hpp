#pragma once

#include <utils/TGBotApi/User/InterfaceUser.hpp>

namespace Utils::TGBotApi::Chat {

using Utils::TGBotApi::User::InterfaceUser;

enum EnumChatType {
    PRIVATE
};

struct InterfaceChat : virtual InterfaceUser {

    virtual EnumChatType get_type() const noexcept = 0;

    ~InterfaceChat() = default;

};

}
