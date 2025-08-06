#pragma once

#include <bot/src/Entity/User.hpp>
#include <utils/TGBotApi/Chat/Chat.hpp>
#include <string>

namespace Bot::Entity {

using Utils::TGBotApi::Chat::EnumChatType;

struct Chat : User {

    EnumChatType type;

    Chat(
        int id,
        optional<datetime> created_at = nullopt,
        optional<datetime> updated_at = nullopt,
        optional<datetime> deleted_at = nullopt,
        long long telegram_id = 0,
        string_view name = "",
        string_view username = "",
        EnumChatType type = EnumChatType::UNKNOWN
    ):
        User(id, created_at, updated_at, deleted_at, telegram_id, name, username),
        type(type)
    {}

};

}