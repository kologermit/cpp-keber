#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/TGBotApi/File/File.hpp>
#include <string_view>
#include <string>

namespace Bot::Entity {

using Utils::Entity::Entity;
using Utils::TGBotApi::File::EnumContentType;
using std::optional;
using std::nullopt;
using jed_utils::datetime;
using std::string_view;
using std::string;


struct Message : Entity {

    long long       telegram_id;
    string          text;
    string          file_download_id;
    string          file_name;
    EnumContentType file_content_type;
    int             chat_id;
    long long       chat_telegram_id;
    int             user_id;
    long long       user_telegram_id;
    int             reply_message_id;
    long long       reply_message_telegram_id;

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
        long long chat_telegram_id = 0,
        int user_id = 0,
        long long user_telegram_id = 0,
        int reply_message_id = 0,
        long long reply_message_telegram_id = 0
    ):
        Entity(id, created_at, updated_at, deleted_at),
        telegram_id(telegram_id),
        text(text),
        file_download_id(file_download_id),
        file_name(file_name),
        file_content_type(file_content_type),
        chat_id(chat_id),
        chat_telegram_id(chat_telegram_id),
        user_id(user_id),
        user_telegram_id(user_telegram_id),
        reply_message_id(reply_message_id),
        reply_message_telegram_id(reply_message_telegram_id)
    {}

};

}