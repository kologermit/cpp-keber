#pragma once

#include <utils/TGBotApi/Chat/InterfaceChat.hpp>
#include <utils/TGBotApi/User/User.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Chat {

using Utils::TGBotApi::User::User;
using nlohmann::json;

struct Chat : virtual InterfaceChat, User  {
    EnumChatType get_type() const noexcept override;
    explicit Chat(const json&);

    private:
    EnumChatType _type;
};

}
