#pragma once

#include <utils/Type.cpp>
#include <utils/TGBotApi/Message/InterfaceMessage.cpp>
#include <utils/TGBotApi/User/InterfaceUser.cpp>
#include <utils/TGBotApi/User/User.cpp>
#include <utils/TGBotApi/Chat/InterfaceChat.cpp>
#include <utils/TGBotApi/Chat/Chat.cpp>
#include <utils/TGBotApi/JSONKeys.cpp>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>

namespace Utils::TGBotApi::Message {

using Utils::Type::const_string;
using Utils::TGBotApi::Chat::InterfaceChat;
using Utils::TGBotApi::Chat::Chat;
using Utils::TGBotApi::User::InterfaceUser;
using Utils::TGBotApi::User::User;
using Utils::TGBotApi::JSONKeys::MESSAGE_ID_KEY;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;
using Utils::TGBotApi::JSONKeys::FROM_KEY;
using Utils::TGBotApi::JSONKeys::CHAT_KEY;
using std::make_shared, std::shared_ptr, std::string;
using nlohmann::json;

struct Message : InterfaceMessage {
    int get_id() const noexcept override;
    string get_text() const noexcept override;
    ptrUser from() const noexcept override;
    ptrChat chat() const noexcept override;

    Message(const json&);

    protected:

    int _id;
    const_string _text;
    ptrUser _from;
    ptrChat _chat;
};

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