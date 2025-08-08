#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/TGBotApi/File/File.hpp>
#include <string_view>
#include <string>
#include <map>
#include <pqxx/pqxx>

namespace Bot::Entity::Message {

using Utils::Entity::Entity;
using Utils::TGBotApi::File::EnumContentType;
using std::optional;
using std::nullopt;
using jed_utils::datetime;
using std::string_view;
using std::string;
using std::map;
using std::to_string;
using pqxx::row;

const map<int, string> map_content_type_to_string{
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumContentType::UNKNOWN),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumContentType::PHOTO),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumContentType::VIDEO),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumContentType::AUDIO),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumContentType::DOCUMENT),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EnumContentType::TEXT),
};

enum MessageColumns {
    TELEGRAM_ID,
    TEXT,
    FILE_DOWNLOAD_ID,
    FILE_NAME,
    FILE_CONTENT_TYPE,
    CHAT_ID,
    USER_ID,
    REPLY_MESSAGE_ID,
};

constexpr const char* MESSAGES_TABLE = "\"messages\"";
constexpr const char* MESSAGE_CONTENT_TYPES_TABLE = "\"message_content_types\"";
constexpr const char* TELEGRAM_ID_COLUMN = "\"telegram_id\"";
constexpr const char* TEXT_COLUMN = "\"text\"";
constexpr const char* FILE_DOWNLOAD_ID_COLUMN = "\"file_download_id\"";
constexpr const char* FILE_NAME_COLUMN = "\"file_name\"";
constexpr const char* FILE_CONTENT_TYPE_COLUMN = "\"file_content_type\"";
constexpr const char* CHAT_ID_COLUMN = "\"chat_id\"";
constexpr const char* USER_ID_COLUMN = "\"user_id\"";
constexpr const char* REPLY_MESSAGE_ID_COLUMN = "\"reply_message_id\"";

struct Message : Entity {

    long long       telegram_id;
    string          text;
    string          file_download_id;
    string          file_name;
    EnumContentType file_content_type;
    int             chat_id;
    int             user_id;
    int             reply_message_id;

    Message(
        int id,
        optional<datetime> created_at = nullopt,
        optional<datetime> updated_at = nullopt,
        optional<datetime> deleted_at = nullopt,
        long long telegram_id = 0,
        string_view text = "",
        string_view file_download_id = "",
        string_view file_name = "",
        EnumContentType file_content_type = EnumContentType::UNKNOWN,
        int chat_id = 0,
        int user_id = 0,
        int reply_message_id = 0
    ):
        Entity(id, created_at, updated_at, deleted_at),
        telegram_id(telegram_id),
        text(text),
        file_download_id(file_download_id),
        file_name(file_name),
        file_content_type(file_content_type),
        chat_id(chat_id),
        user_id(user_id),
        reply_message_id(reply_message_id)
    {}

    Message(
        long long telegram_id = 0,
        string_view text = "",
        string_view file_download_id = "",
        string_view file_name = "",
        EnumContentType file_content_type = EnumContentType::UNKNOWN,
        int chat_id = 0,
        int user_id = 0,
        int reply_message_id = 0
    ):
        telegram_id(telegram_id),
        text(text),
        file_download_id(file_download_id),
        file_name(file_name),
        file_content_type(file_content_type),
        chat_id(chat_id),
        user_id(user_id),
        reply_message_id(reply_message_id)
    {}

    Message(const row& message_row):
    Entity(message_row),
        telegram_id(message_row[TELEGRAM_ID_COLUMN].get<long long>().value()),
        text(message_row[TEXT_COLUMN].get<string>().value()),
        file_download_id(message_row[FILE_DOWNLOAD_ID_COLUMN].get<string>().value()),
        file_name(message_row[FILE_NAME_COLUMN].get<string>().value()),
        file_content_type(static_cast<EnumContentType>(message_row[FILE_CONTENT_TYPE_COLUMN].get<int>().value())),
        chat_id(message_row[CHAT_ID_COLUMN].get<int>().value()),
        user_id(message_row[USER_ID_COLUMN].get<int>().value()),
        reply_message_id(
            message_row[REPLY_MESSAGE_ID_COLUMN].is_null()
            ? 0
            : message_row[REPLY_MESSAGE_ID_COLUMN].get<int>().value()
        )
    {}

    string get_data_by_column(int column) const {
        switch (column)
        {
        case TELEGRAM_ID:
            return to_string(telegram_id);
            break;
        case TEXT:
            return text;
            break; 
        case FILE_DOWNLOAD_ID:
            return file_download_id;
            break;
        case FILE_NAME:
            return file_name;
            break;
        case FILE_CONTENT_TYPE:
            return to_string(static_cast<int>(file_content_type));
            break;
        case CHAT_ID:
            return to_string(chat_id);
            break;
        case USER_ID:
            return to_string(user_id);
            break;
        case REPLY_MESSAGE_ID:
            return to_string(reply_message_id);
            break;
        default:
            return "";
            break;
        }
    }

};

}