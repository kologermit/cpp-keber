#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/TGBotApi/File/File.hpp>
#include <string_view>
#include <string>
#include <map>
#include <pqxx/pqxx>

namespace Bot::Entity::Message {

    using Utils::Entity::Entity;
    using Utils::Entity::Column;
    using Utils::TGBotApi::File::EnumContentType;
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
        {EnumContentType::UNKNOWN, "UNKNOWN"},
        {EnumContentType::PHOTO, "PHOTO"},
        {EnumContentType::VIDEO, "VIDEO"},
        {EnumContentType::AUDIO, "AUDIO"},
        {EnumContentType::DOCUMENT, "DOCUMENT"},
        {EnumContentType::TEXT, "TEXT"},
    };

    const auto TEXT = make_shared<Column>("text");
    const auto FILE_DOWNLOAD_ID = make_shared<Column>("file_download_id");
    const auto FILE_NAME = make_shared<Column>("file_name");
    const auto FILE_CONTENT_TYPE = make_shared<Column>("file_type");
    const auto CHAT_ID = make_shared<Column>("chat_id");
    const auto USER_ID = make_shared<Column>("user_id");
    const auto REPLY_MESSAGE_ID = make_shared<Column>("reply_message_id");

    constexpr const char* MESSAGE_CONTENT_TYPES_TABLE = "message_content_types";

    struct Message : Entity {

        string          text;
        string          file_download_id;
        string          file_name;
        EnumContentType file_content_type;
        long long       chat_id;
        long long       user_id;
        long long       reply_message_id;

        Message(
            string_view text = "",
            string_view file_download_id = "",
            string_view file_name = "",
            EnumContentType file_content_type = EnumContentType::UNKNOWN,
            long long chat_id = 0,
            long long user_id = 0,
            long long reply_message_id = 0,
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
            reply_message_id(reply_message_id)
        {}

        Message(const row& message_row):
        Entity(message_row),
            text(message_row[TEXT->name].get<string>().value()),
            file_download_id(message_row[FILE_DOWNLOAD_ID->name].get<string>().value()),
            file_name(message_row[FILE_NAME->name].get<string>().value()),
            file_content_type(static_cast<EnumContentType>(message_row[FILE_CONTENT_TYPE->name].get<int>().value())),
            chat_id(message_row[CHAT_ID->name].get<int>().value()),
            user_id(message_row[USER_ID->name].get<int>().value()),
            reply_message_id(
                message_row[REPLY_MESSAGE_ID->name].is_null()
                ? 0
                : message_row[REPLY_MESSAGE_ID->name].get<int>().value()
            )
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "messages";
            return table;
        }
        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);

            result[TEXT->name] = text;
            result[FILE_DOWNLOAD_ID->name] = file_download_id;
            result[FILE_NAME->name] = file_name;
            result[CHAT_ID->name] = to_string(chat_id);
            result[USER_ID->name] = to_string(user_id);
            result[REPLY_MESSAGE_ID->name] = to_string(reply_message_id);

            return result;
        }
    };
}