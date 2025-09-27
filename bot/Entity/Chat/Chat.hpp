#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/TGBotApi/Chat/Chat.hpp>
#include <string>
#include <map>

namespace Bot::Entity::Chat {

    using Utils::TGBotApi::Chat::EnumChatType;
    using Utils::Entity::Column;
    using Utils::Entity::Entity;
    using std::optional;
    using std::nullopt;
    using std::string_view;
    using std::string;
    using std::to_string;
    using std::map;
    using std::make_shared;
    using jed_utils::datetime;
    using pqxx::row;

    constexpr const char* CHAT_TYPES_TABLE = "chat_types";
    const auto NAME = make_shared<Column>("name");
    const auto USERNAME = make_shared<Column>("username");
    const auto TYPE = make_shared<Column>("type");

    const map<int, string> map_chat_type_to_string{
        {EnumChatType::UNKNOWN, "UNKNOWN"},
        {EnumChatType::PRIVATE, "PRIVATE"},
        {EnumChatType::CHANNEL, "CHANNEL"},
        {EnumChatType::GROUP, "GROUP"},
        {EnumChatType::SUPERGROUP, "SUPERGROUP"},
    };

    struct Chat : Entity {

        string name;
        string username;
        EnumChatType type;

        Chat(
            string_view name = "",
            string_view username = "",
            EnumChatType type = EnumChatType::UNKNOWN,
            long long id = 0,
            datetime created_at = {},
            datetime updated_at = {},
            optional<datetime> deleted_at = nullopt
        ):
            Entity(id, created_at, updated_at, deleted_at),
            name(name),
            username(username),
            type(type)
        {}

        Chat(const row& chat_row):
            Entity(chat_row),
            name(chat_row[NAME->name].get<string>().value()),
            username(chat_row[USERNAME->name].get<string>().value()),
            type(static_cast<EnumChatType>(chat_row[TYPE->name].get<int>().value()))
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "chats";
            return table;
        }

        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);

            result[NAME->name] = name;
            result[USERNAME->name] = username;
            result[TYPE->name] = to_string(type);

            return result;
        }
    };

}