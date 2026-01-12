#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/TGBotApi/Chat/Chat.hpp>
#include <string>
#include <map>

namespace Bot::Entity::Chat {

    using Utils::TGBotApi::Chat::ChatType;
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
    constexpr const char* NAME_COLUMN = "name";
    constexpr const char* USERNAME_COLUMN = "username";
    constexpr const char* TYPE_COLUMN = "type";

    const map<int, string> map_chat_type_to_string{
        {ChatType::UNKNOWN, "UNKNOWN"},
        {ChatType::PRIVATE, "PRIVATE"},
        {ChatType::CHANNEL, "CHANNEL"},
        {ChatType::GROUP, "GROUP"},
        {ChatType::SUPERGROUP, "SUPERGROUP"},
    };

    struct Chat : Entity {

        string name;
        string username;
        ChatType type;

        explicit Chat(
            string_view name = "",
            string_view username = "",
            ChatType type = ChatType::UNKNOWN,
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

        explicit Chat(const row& chat_row):
            Entity(chat_row),
            name(chat_row[NAME_COLUMN].get<string>().value()),
            username(chat_row[USERNAME_COLUMN].get<string>().value()),
            type(static_cast<ChatType>(chat_row[TYPE_COLUMN].get<int>().value()))
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "chats";
            return table;
        }

        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);

            result[NAME_COLUMN] = name;
            result[USERNAME_COLUMN] = username;
            result[TYPE_COLUMN] = to_string(type);

            return result;
        }
    };

}