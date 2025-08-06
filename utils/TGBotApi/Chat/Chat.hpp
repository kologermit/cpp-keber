#pragma once

#include <utils/TGBotApi/User/User.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Chat {

enum EnumChatType {
    PRIVATE,
    GROUP,
    SUPERGROUP,
    CHANNEL,
    UNKNOWN
};

using Utils::TGBotApi::User::User;
using nlohmann::json;
using std::string_view;

struct Chat : User  {

    Chat(const json&);
    Chat(
        long long id,
        string_view name, 
        string_view username = "",
        EnumChatType type = EnumChatType::PRIVATE
    );

    EnumChatType type;
};

}
