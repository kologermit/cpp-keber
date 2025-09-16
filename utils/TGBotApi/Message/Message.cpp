#include <utils/TGBotApi/Message/Message.hpp>
#include <utils/TGBotApi/Chat/Chat.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
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
    using std::make_unique, std::move;

    Message::Message(const json& json_message):
    id(json_message[MESSAGE_ID_KEY]),
    text(
        json_message.contains(TEXT_KEY)
        ? string(json_message[TEXT_KEY])
        : json_message.value(CAPTION_KEY, "")
    ),
    file_download_id(
        // document
        json_message.contains(DOCUMENT_KEY)
        ? json_message[DOCUMENT_KEY][FILE_ID_KEY]
        // photo
        : json_message.contains(PHOTO_KEY)
        ? json_message[PHOTO_KEY].back()[FILE_ID_KEY]
        // video
        : json_message.contains(VIDEO_KEY)
        ? json_message[VIDEO_KEY][FILE_ID_KEY]
        // audio
        : json_message.contains(AUDIO_KEY)
        ? json_message[AUDIO_KEY][FILE_ID_KEY]

        : ""
    ),
    file_content_type(
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

        : EnumContentType::TEXT
    ),
    file_name(
        json_message.contains(DOCUMENT_KEY) ? string(json_message[DOCUMENT_KEY][FILE_NAME_KEY])
        : json_message.contains(PHOTO_KEY) ? format(
            "{}_{}.jpg",
            static_cast<long long>(json_message[FROM_KEY][ID_KEY]),
            static_cast<long long>(json_message[MESSAGE_ID_KEY])
        )
        : json_message.contains(VIDEO_KEY) ? string(json_message[VIDEO_KEY][FILE_NAME_KEY])
        : json_message.contains(AUDIO_KEY) ? string(json_message[AUDIO_KEY][FILE_NAME_KEY])
        : ""
    ),
    file_size(
        json_message.contains(DOCUMENT_KEY) ? static_cast<long long>(json_message[DOCUMENT_KEY][FILE_SIZE_KEY])
        : json_message.contains(PHOTO_KEY) ? static_cast<long long>(json_message[PHOTO_KEY].back()[FILE_SIZE_KEY])
        : json_message.contains(VIDEO_KEY) ? static_cast<long long>(json_message[VIDEO_KEY][FILE_SIZE_KEY])
        : json_message.contains(AUDIO_KEY) ? static_cast<long long>(json_message[AUDIO_KEY][FILE_SIZE_KEY])
        : 0
    ),
    from(make_unique<User>(json_message[FROM_KEY])),
    chat(make_unique<Chat>(json_message[CHAT_KEY])),
    reply_message(
        json_message.contains(REPLY_TO_MESSAGE_KEY)
        ? make_unique<Message>(json_message[REPLY_TO_MESSAGE_KEY])
        : nullptr
    ) {}

    Message::Message(
        long long         id,
        string_view       text,
        string_view       file_download_id,
        EnumContentType   file_content_type,
        string_view       file_name,
        long long         file_size,
        unique_ptr<User>    from,
        unique_ptr<Chat>    chat,
        unique_ptr<Message> reply_message
    ):
    id(id),
    text(text),
    file_download_id(file_download_id),
    file_content_type(file_content_type),
    file_name(file_name),
    file_size(file_size),
    from(std::move(from)),
    chat(std::move(chat)),
    reply_message(std::move(reply_message))
    {}
}
