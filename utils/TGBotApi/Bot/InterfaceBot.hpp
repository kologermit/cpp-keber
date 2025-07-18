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
using Utils::Types::optional_string_view;
using Utils::Types::optional_const_string;
using Utils::Types::optional_int;
using Utils::TGBotApi::User::InterfaceUser;
using Utils::TGBotApi::Message::InterfaceMessage;
using Utils::TGBotApi::File::EnumContentType;
using Utils::TGBotApi::JSONKeys::HTML_KEY;
using Utils::TGBotApi::Message::Keyboard::InterfaceInlineKeyboard;
using Utils::TGBotApi::Message::Keyboard::InterfaceReplyKeyboard;

struct SendMessageParameters {
    using ptrInlineKeyboard = shared_ptr<InterfaceInlineKeyboard>;
    using ptrReplyKeyboard = shared_ptr<InterfaceReplyKeyboard>;

    int chat_id;
    optional_const_string text;
    optional_int reply_message_id;
    optional_const_string placeholder;
    optional_const_string filepath;
    optional_const_string filename;
    EnumContentType content_type = EnumContentType::TEXT;
    const_string parse_mode = HTML_KEY;
    ptrInlineKeyboard inline_keyboard;
    ptrReplyKeyboard reply_keyboard;
};

struct InterfaceBot : virtual InterfaceUser {
    virtual bool can_join_groups() const noexcept = 0;
    virtual bool can_read_all_group_messages() const noexcept = 0;
    virtual bool supports_inline_queries() const noexcept = 0;
    virtual bool can_connect_to_business() const noexcept = 0;
    virtual bool has_main_web_app() const noexcept = 0;

    using ptrMessage = shared_ptr<InterfaceMessage>;
    
    virtual ptrMessage send_message(const SendMessageParameters& message_data) const = 0;
    virtual void delete_webhook() const = 0;
    virtual void set_webhook(string_view url) const = 0;

    ~InterfaceBot() = default;
};

using ptrBot = shared_ptr<InterfaceBot>;

}
