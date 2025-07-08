#pragma once

#include <utils/TGBotApi/Message/Message.hpp>
#include <utils/TGBotApi/Chat/Chat.cpp>

namespace Utils::TGBotApi::Message {

using Utils::TGBotApi::Chat::Chat;
using Utils::TGBotApi::User::User;
using Utils::TGBotApi::JSONKeys::MESSAGE_ID_KEY;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;
using Utils::TGBotApi::JSONKeys::FROM_KEY;
using Utils::TGBotApi::JSONKeys::CHAT_KEY;
using std::make_shared;

Message::Message(const json& json_message):
_id(json_message[MESSAGE_ID_KEY]),
_text(json_message[TEXT_KEY]),
_from(make_shared<User>(json_message[FROM_KEY])),
_chat(make_shared<Chat>(json_message[CHAT_KEY])) {}

int Message::get_id() const noexcept {
    return _id;
}

Message::ptrUser Message::from() const noexcept {
    return _from;
}

Message::ptrChat Message::chat() const noexcept {
    return _chat;
}

string Message::get_text() const noexcept {
    return _text;
}

}
