#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/TGBotApi/File/File.hpp>
#include <string_view>
#include <string>
#include <map>
#include <pqxx/pqxx>

namespace Bot::Entity::Message {

    using Utils::Entity::Entity;
    using Utils::TGBotApi::File::ContentType;
    using std::optional;
    using std::nullopt;
    using jed_utils::datetime;
    using std::string_view;
    using std::string;
    using std::map;
    using std::to_string;
    using pqxx::row;
    using std::make_shared;

    const map<int, string> map_content_type_to_string{
        {ContentType::UNKNOWN, "UNKNOWN"},
        {ContentType::PHOTO, "PHOTO"},
        {ContentType::VIDEO, "VIDEO"},
        {ContentType::AUDIO, "AUDIO"},
        {ContentType::DOCUMENT, "DOCUMENT"},
        {ContentType::TEXT, "TEXT"},
    };

    constexpr const char* TEXT_COLUMN = "text";
    constexpr const char* FILE_DOWNLOAD_ID_COLUMN = "file_download_id";
    constexpr const char* FILE_NAME_COLUMN = "file_name";
    constexpr const char* FILE_CONTENT_TYPE_COLUMN = "file_content_type";
    constexpr const char* CHAT_ID_COLUMN = "chat_id";
    constexpr const char* USER_ID_COLUMN = "user_id";
    constexpr const char* REPLY_MESSAGE_ID_COLUMN = "reply_message_id";
    constexpr const char* REPLY_CHAT_ID_COLUMN = "reply_chat_id";

    constexpr const char* MESSAGE_CONTENT_TYPES_TABLE = "message_content_types";

    struct Message : Entity {

        string          text;
        string          file_download_id;
        string          file_name;
        ContentType file_content_type;
        long long       chat_id;
        long long       user_id;
        long long       reply_message_id;
        long long       reply_chat_id;

        explicit Message(
            string_view text = "",
            string_view file_download_id = "",
            string_view file_name = "",
            ContentType file_content_type = ContentType::UNKNOWN,
            long long chat_id = 0,
            long long user_id = 0,
            long long reply_message_id = 0,
            long long reply_chat_id = 0,
            long long id = 0,
            datetime created_at = {},
            datetime updated_at = {},
            optional<datetime> deleted_at = nullopt
        ):
            Entity(id, created_at, updated_at, deleted_at),
            text(text),
            file_download_id(file_download_id),
            file_name(file_name),
            file_content_type(file_content_type),
            chat_id(chat_id),
            user_id(user_id),
            reply_message_id(reply_message_id),
            reply_chat_id(reply_chat_id)
        {}

        explicit Message(const row& message_row):
        Entity(message_row),
            text(message_row[TEXT_COLUMN].get<string>().value()),
            file_download_id(message_row[FILE_DOWNLOAD_ID_COLUMN].get<string>().value()),
            file_name(message_row[FILE_NAME_COLUMN].get<string>().value()),
            file_content_type(static_cast<ContentType>(message_row[FILE_CONTENT_TYPE_COLUMN].get<int>().value())),
            chat_id(message_row[CHAT_ID_COLUMN].get<long long>().value()),
            user_id(message_row[USER_ID_COLUMN].get<long long>().value()),
            reply_message_id(
                message_row[REPLY_MESSAGE_ID_COLUMN].is_null()
                ? 0
                : message_row[REPLY_MESSAGE_ID_COLUMN].get<long long>().value()
            ),
            reply_chat_id(
                message_row[REPLY_CHAT_ID_COLUMN].is_null()
                ? 0
                : message_row[REPLY_CHAT_ID_COLUMN].get<long long>().value()
            )
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "messages";
            return table;
        }
        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);

            result[TEXT_COLUMN] = text;
            result[FILE_DOWNLOAD_ID_COLUMN] = file_download_id;
            result[FILE_NAME_COLUMN] = file_name;
            result[FILE_CONTENT_TYPE_COLUMN] = to_string(static_cast<int>(file_content_type));
            result[CHAT_ID_COLUMN] = to_string(chat_id);
            result[USER_ID_COLUMN] = to_string(user_id);
            if (reply_message_id != 0) {
                result[REPLY_MESSAGE_ID_COLUMN] = to_string(reply_message_id);
                result[REPLY_CHAT_ID_COLUMN] = to_string(reply_chat_id);
            }

            return result;
        }
    };
}