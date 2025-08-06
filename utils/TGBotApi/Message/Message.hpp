#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/File/File.hpp>
#include <utils/TGBotApi/User/User.hpp>
#include <utils/TGBotApi/Chat/Chat.hpp>

namespace Utils::TGBotApi::Message {

using std::string_view;
using std::string;
using std::unique_ptr;
using Utils::TGBotApi::File::EnumContentType;
using Utils::TGBotApi::Chat::Chat;
using Utils::TGBotApi::User::User;
using nlohmann::json;

struct Message {

    Message(
        long long         id,
        string_view       text = "",
        string_view       file_download_id = "",
        EnumContentType   file_content_type = EnumContentType::TEXT,
        string_view       file_name = "",
        long long         file_size = 0,
        unique_ptr<User>    from = nullptr,
        unique_ptr<Chat>    chat = nullptr,
        unique_ptr<Message> reply_message = nullptr
    );

    Message(const json&);

    long long         id;
    string            text;
    string            file_download_id;
    EnumContentType   file_content_type;
    string            file_name;
    long long         file_size;
    unique_ptr<User>    from;
    unique_ptr<Chat>    chat;
    unique_ptr<Message> reply_message;
};

}
