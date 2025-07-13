#include <utils/TGBotApi/Message/Message.hpp>
#include <utils/TGBotApi/Chat/Chat.cpp>

namespace Utils::TGBotApi::Message {

using Utils::TGBotApi::Chat::Chat;
using Utils::TGBotApi::User::User;
using Utils::TGBotApi::JSONKeys::MESSAGE_ID_KEY;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;
using Utils::TGBotApi::JSONKeys::FROM_KEY;
using Utils::TGBotApi::JSONKeys::CHAT_KEY;
using Utils::TGBotApi::JSONKeys::CAPTION_KEY;
using Utils::TGBotApi::JSONKeys::FILE_ID_KEY;
using Utils::TGBotApi::JSONKeys::PHOTO_KEY;
using Utils::TGBotApi::JSONKeys::DOCUMENT_KEY;
using Utils::Types::optional_string_view;
using std::make_shared, std::nullopt;

Message::Message(const json& json_message):
_id(json_message[MESSAGE_ID_KEY]),
_text(
    json_message.contains(TEXT_KEY)
    ? optional_const_string(json_message[TEXT_KEY])
    : json_message.contains(CAPTION_KEY)
        ? optional_const_string(json_message[CAPTION_KEY])
        : nullopt
),
_photo_id(
    json_message.contains(PHOTO_KEY)
    ? optional_const_string(json_message[PHOTO_KEY][4][FILE_ID_KEY])
    : nullopt
),
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

optional_const_string Message::get_text() const noexcept {
    return _text;
}

optional_const_string Message::get_photo_id() const noexcept {
    return _photo_id;
}

optional_const_string Message::get_document_id() const noexcept {
    return _document_id;
}

optional_const_string Message::get_document_name() const noexcept {
    return _document_name;
}

}
