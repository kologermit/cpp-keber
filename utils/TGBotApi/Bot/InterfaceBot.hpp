#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <utils/Types.hpp>
#include <utils/TGBotApi/Message/Keyboard/InterfaceInlineKeyboard.hpp>
#include <utils/TGBotApi/Message/Keyboard/InterfaceReplyKeyboard.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/TGBotApi/File/File.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>
#include <utils/TGBotApi/Message/InterfaceMessage.hpp>

namespace Utils::TGBotApi::Bot {

using std::string_view, std::shared_ptr, std::nullopt, std::optional;
using Utils::Types::const_string;
using Utils::Types::const_c_string;
using Utils::Types::optional_const_string;
using Utils::Types::optional_long_long;
using Utils::TGBotApi::User::InterfaceUser;
using Utils::TGBotApi::Message::InterfaceMessage;
using Utils::TGBotApi::File::EnumContentType;
using Utils::TGBotApi::JSONKeys::HTML_KEY;
using Utils::TGBotApi::Message::Keyboard::InterfaceInlineKeyboard;
using Utils::TGBotApi::Message::Keyboard::InterfaceReplyKeyboard;

constexpr const_c_string SECRET_HEADER = "X-Telegram-Bot-Api-Secret-Token";

struct SendMessageParameters {
    long long                           chat_id;
    optional_const_string               text;
    optional_long_long                  reply_message_id;
    optional_const_string               placeholder;
    optional_const_string               filepath;
    optional_const_string               filename;
    EnumContentType                     content_type      = EnumContentType::TEXT;
    const_string                        parse_mode        = HTML_KEY;
    shared_ptr<InterfaceInlineKeyboard> inline_keyboard;
    shared_ptr<InterfaceReplyKeyboard>  reply_keyboard;
};

struct InterfaceBot : virtual InterfaceUser {
    virtual bool                         can_join_groups()                                                                                          const noexcept = 0;
    virtual bool                         can_read_all_group_messages()                                                                              const noexcept = 0;
    virtual bool                         supports_inline_queries()                                                                                  const noexcept = 0;
    virtual bool                         can_connect_to_business()                                                                                  const noexcept = 0;
    virtual bool                         has_main_web_app()                                                                                         const noexcept = 0;
    virtual shared_ptr<InterfaceMessage> send_message(const SendMessageParameters& message_data)                                                             const = 0;
    virtual shared_ptr<InterfaceMessage> edit_text(long long chat_id, long long message_id, string_view text)                                                const = 0;
    virtual shared_ptr<InterfaceMessage> edit_caption(long long chat_id, long long message_id, string_view caption)                                          const = 0;
    virtual bool                         delete_message(long long chat_id, long long message_id)                                                             const = 0;
    virtual bool                         delete_webhook()                                                                                                    const = 0;
    virtual bool                         set_webhook(string_view url)                                                                                        const = 0;
    virtual bool                         answer_callback_query(string_view callback_query_id, optional_const_string text = nullopt, bool show_alert = false) const = 0;
    virtual bool                         check_secret_token(string_view secret_token)                                                               const noexcept = 0;

    ~InterfaceBot() = default;
};

}
