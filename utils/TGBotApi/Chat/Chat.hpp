#pragma once

#include <utils/TGBotApi/User/User.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Chat {

enum ChatType {
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
    explicit Chat(const json&);

    explicit Chat(
        long long id,
        string_view name, 
        string_view username = "",
        ChatType type = ChatType::PRIVATE
    );

    ChatType type;
};

}
