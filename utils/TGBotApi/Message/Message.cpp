#include <utils/TGBotApi/Message/Message.hpp>
#include <utils/TGBotApi/Chat/Chat.cpp>
#include <fmt/core.h>

namespace Utils::TGBotApi::Message {

using Utils::TGBotApi::Chat::Chat;
using Utils::TGBotApi::User::User;
using Utils::TGBotApi::JSONKeys::AUDIO_KEY;
using Utils::TGBotApi::JSONKeys::CAPTION_KEY;
using Utils::TGBotApi::JSONKeys::CHAT_KEY;
using Utils::TGBotApi::JSONKeys::DOCUMENT_KEY;
using Utils::TGBotApi::JSONKeys::FILE_ID_KEY;
using Utils::TGBotApi::JSONKeys::FILE_NAME_KEY;
using Utils::TGBotApi::JSONKeys::FILE_SIZE_KEY;
using Utils::TGBotApi::JSONKeys::FROM_KEY;
using Utils::TGBotApi::JSONKeys::ID_KEY;
using Utils::TGBotApi::JSONKeys::MESSAGE_ID_KEY;
using Utils::TGBotApi::JSONKeys::REPLY_TO_MESSAGE_KEY;
using Utils::TGBotApi::JSONKeys::PHOTO_KEY;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;
using Utils::TGBotApi::JSONKeys::VIDEO_KEY;
using fmt::format;
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
_file_download_id(
    // document
    json_message.contains(DOCUMENT_KEY)
    ? optional_const_string(json_message[DOCUMENT_KEY][FILE_ID_KEY])
    // photo
    : json_message.contains(PHOTO_KEY)
    ? optional_const_string(json_message[PHOTO_KEY].back()[FILE_ID_KEY])
    // video
    : json_message.contains(VIDEO_KEY)
    ? optional_const_string(json_message[VIDEO_KEY][FILE_ID_KEY])
    // audio
    : json_message.contains(AUDIO_KEY)
    ? optional_const_string(json_message[AUDIO_KEY][FILE_ID_KEY])

    : nullopt
),
_file_content_type(
    // document
    json_message.contains(DOCUMENT_KEY)
    ? EnumContentType::DOCUMENT
    // photo
    : json_message.contains(PHOTO_KEY)
    ? EnumContentType::PHOTO
    // video
    : json_message.contains(VIDEO_KEY)
    ? EnumContentType::VIDEO
    // audio
    : json_message.contains(AUDIO_KEY)
    ? EnumContentType::AUDIO

    : EnumContentType::UNKNOWN
),
_file_name(
    // document
    json_message.contains(DOCUMENT_KEY)
    ? optional_const_string(json_message[DOCUMENT_KEY][FILE_NAME_KEY])
    // photo
    : json_message.contains(PHOTO_KEY)
    ? optional_const_string(format(
        "{}_{}.jpg",
        int(json_message[FROM_KEY][ID_KEY]),
        int(json_message[MESSAGE_ID_KEY])
    ))
    // video
    : json_message.contains(VIDEO_KEY)
    ? optional_const_string(json_message[VIDEO_KEY][FILE_NAME_KEY])
    // audio
    : json_message.contains(AUDIO_KEY)
    ? optional_const_string(json_message[AUDIO_KEY][FILE_NAME_KEY])

    : nullopt
),
_file_size(
    // document
    json_message.contains(DOCUMENT_KEY)
    ? optional_int(json_message[DOCUMENT_KEY][FILE_SIZE_KEY])
    // photo
    : json_message.contains(PHOTO_KEY)
    ? optional_int(json_message[PHOTO_KEY].back()[FILE_SIZE_KEY])
    // video
    : json_message.contains(VIDEO_KEY)
    ? optional_int(json_message[VIDEO_KEY][FILE_SIZE_KEY])
    // audio
    : json_message.contains(AUDIO_KEY)
    ? optional_int(json_message[AUDIO_KEY][FILE_SIZE_KEY])

    : nullopt
),
_reply_message(
    json_message.contains(REPLY_TO_MESSAGE_KEY)
    ? make_shared<Message>(json_message[REPLY_TO_MESSAGE_KEY])
    : nullptr
),
_from(make_shared<User>(json_message[FROM_KEY])),
_chat(make_shared<Chat>(json_message[CHAT_KEY])) {}

int Message::get_id() const noexcept {
    return _id;
}

Message::ptrUser Message::get_from() const noexcept {
    return _from;
}

Message::ptrChat Message::get_chat() const noexcept {
    return _chat;
}

optional_const_string Message::get_text() const noexcept {
    return _text;
}

optional_const_string Message::get_file_download_id() const noexcept {
    return _file_download_id;
}

EnumContentType Message::get_file_content_type() const noexcept {
    return _file_content_type;
}

optional_const_string Message::get_file_name() const noexcept {
    return _file_name;
}

optional_int Message::get_file_size() const noexcept {
    return _file_size;
}

InterfaceMessage::ptrMessage Message::get_reply_message() const noexcept {
    return _reply_message;
}

}
