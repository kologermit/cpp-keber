#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/TGBotApi/Chat/Chat.hpp>
#include <string>
#include <map>

namespace Bot::Entity::Chat {

    using Utils::TGBotApi::Chat::EnumChatType;
    using Utils::Entity::Entity;
    using std::optional;
    using std::nullopt;
    using std::string_view;
    using std::string;
    using std::to_string;
    using std::map;
    using jed_utils::datetime;
    using pqxx::row;

    constexpr const char* CHAT_TYPES_TABLE = "\"chat_types\"";
    constexpr const char* CHATS_TABLE = "\"chats\"";
    constexpr const char* TELEGRAM_ID_COLUMN = "\"telegram_id\"";
    constexpr const char* NAME_COLUMN = "\"name\"";
    constexpr const char* USERNAME_COLUMN = "\"username\"";
    constexpr const char* TYPE_COLUMN = "\"type\"";

    const map<int, string> map_chat_type_to_string{
        GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumChatType::UNKNOWN),
        GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumChatType::PRIVATE),
        GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumChatType::CHANNEL),
        GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumChatType::GROUP),
        GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumChatType::SUPERGROUP),
    };

    enum ChatColumns {
        TELEGRAM_ID,
        NAME,
        USERNAME,
        TYPE,
    };

    struct Chat : Entity {

        long long telegram_id;
        string name;
        string username;
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
            Entity(id, created_at, updated_at, deleted_at),
            telegram_id(telegram_id),
            name(name),
            username(username),
            type(type)
        {}

        Chat(
            long long telegram_id = 0,
            string_view name = "",
            string_view username = "",
            EnumChatType type = EnumChatType::UNKNOWN
        ):
            Entity(0, nullopt, nullopt, nullopt),
            telegram_id(telegram_id),
            name(name),
            username(username),
            type(type)
        {}

        Chat(const row& chat_row):
            Entity(chat_row),
            telegram_id(chat_row[TELEGRAM_ID_COLUMN].get<long long>().value()),
            name(chat_row[NAME_COLUMN].get<string>().value()),
            username(chat_row[USERNAME_COLUMN].get<string>().value()),
            type(static_cast<EnumChatType>(chat_row[TYPE_COLUMN].get<int>().value()))
        {}

        string get_data_by_column(int column) const {
            switch (column)
            {
                case TELEGRAM_ID: return to_string(telegram_id);
                case NAME: return name;
                case USERNAME: return username;
                case TYPE:  return to_string(type);
                default: return "";
            }
        }
    };

}