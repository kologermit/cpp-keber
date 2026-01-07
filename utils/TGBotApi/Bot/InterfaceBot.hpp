#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <utils/TGBotApi/Message/Keyboard/InlineKeyboard.hpp>
#include <utils/TGBotApi/Message/Keyboard/ReplyKeyboard.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/TGBotApi/File/File.hpp>
#include <utils/TGBotApi/User/User.hpp>
#include <utils/TGBotApi/Message/Message.hpp>

namespace Utils::TGBotApi::Bot {

    using std::string_view, std::string, std::unique_ptr, std::shared_ptr;
    using Utils::TGBotApi::User::User;
    using Utils::TGBotApi::Message::Message;
    using Utils::TGBotApi::File::EnumContentType;
    using Utils::TGBotApi::JSONKeys::HTML_KEY;
    using Utils::TGBotApi::Message::Keyboard::InlineKeyboard;
    using Utils::TGBotApi::Message::Keyboard::ReplyKeyboard;

    constexpr const char* SECRET_HEADER = "X-Telegram-Bot-Api-Secret-Token";

    struct SendMessageParameters {
        long long chat_id = 0;
        string text = "";
        long long reply_message_id = 0;
        string placeholder = "";
        string filepath = "";
        string filename = "";
        EnumContentType content_type = EnumContentType::TEXT;
        string parse_mode = HTML_KEY;
        unique_ptr<InlineKeyboard> inline_keyboard = nullptr;
        unique_ptr<ReplyKeyboard> reply_keyboard = nullptr;
    };

    struct InterfaceBot {

        virtual long long get_id() const noexcept = 0;
        virtual const User& get_user() const noexcept = 0;
        virtual const string& get_name() const noexcept = 0;
        virtual const string& get_username() const noexcept = 0;
        virtual bool can_join_groups() const noexcept = 0;
        virtual bool can_read_all_group_messages() const noexcept = 0;
        virtual bool supports_inline_queries() const noexcept = 0;
        virtual bool can_connect_to_business() const noexcept = 0;
        virtual bool has_main_web_app() const noexcept = 0;
        virtual unique_ptr<Message> send_message(const SendMessageParameters& message_data) const = 0;
        virtual unique_ptr<Message> edit_text(long long chat_id, long long message_id, string_view text) const = 0;
        virtual unique_ptr<Message> edit_caption(long long chat_id, long long message_id, string_view caption) const = 0;
        virtual bool delete_message(long long chat_id, long long message_id) const = 0;
        virtual bool delete_webhook() const = 0;
        virtual bool set_webhook(string_view url) const = 0;
        virtual bool answer_callback_query(
            string_view callback_query_id, 
            string_view text = "", 
            bool show_alert = false
        ) const = 0;
        virtual bool check_secret_token(string_view secret_token) const noexcept = 0;
        virtual bool download_file(string_view file_id, string_view output_path) const = 0;

        virtual ~InterfaceBot() = default;
    };
}
