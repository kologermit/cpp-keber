#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <httplib.h>
#include <utils/UUID4/UUID4.hpp>
#include <utils/TGBotApi/Query/Query.hpp>
#include <utils/TGBotApi/Message/Message.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/TGBotApi/Bot/Bot.hpp>
#include <utils/TGBotApi/File/File.hpp>
#include <iostream>

namespace Utils::TGBotApi::Bot {

using Utils::TGBotApi::Query::Query;
using Utils::TGBotApi::Query::EnumQueryMethod;
using Utils::TGBotApi::Message::Message;
using Utils::TGBotApi::JSONKeys::RESULT_KEY;
using Utils::TGBotApi::JSONKeys::SECRET_TOKEN_KEY;
using Utils::TGBotApi::JSONKeys::CHAT_ID_KEY;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;
using Utils::TGBotApi::JSONKeys::TRUE_KEY;
using Utils::TGBotApi::JSONKeys::SHOW_ALERT_KEY;
using Utils::TGBotApi::JSONKeys::URL_KEY;
using Utils::TGBotApi::JSONKeys::CAN_JOIN_GROUPS_KEY;
using Utils::TGBotApi::JSONKeys::CAN_READ_ALL_GROUP_MESSAGES_KEY;
using Utils::TGBotApi::JSONKeys::SUPPORTS_INLINE_QUERIES_KEY;
using Utils::TGBotApi::JSONKeys::CAN_CONNECT_TO_BUSINESS_KEY;
using Utils::TGBotApi::JSONKeys::HAS_MAIN_WEB_APP_KEY;
using Utils::TGBotApi::JSONKeys::REPLY_TO_MESSAGE_ID_KEY;
using Utils::TGBotApi::JSONKeys::CAPTION_KEY;
using Utils::TGBotApi::JSONKeys::HTML_KEY;
using Utils::TGBotApi::JSONKeys::PLACEHOLDER_KEY;
using Utils::TGBotApi::JSONKeys::PARSE_MODE_KEY;
using Utils::TGBotApi::JSONKeys::DOCUMENT_KEY;
using Utils::TGBotApi::JSONKeys::PHOTO_KEY;
using Utils::TGBotApi::JSONKeys::AUDIO_KEY;
using Utils::TGBotApi::JSONKeys::VIDEO_KEY;
using Utils::TGBotApi::JSONKeys::MESSAGE_ID_KEY;
using Utils::TGBotApi::JSONKeys::ALLOWED_UPDATES_KEY;
using Utils::TGBotApi::JSONKeys::MESSAGE_KEY;
using Utils::TGBotApi::JSONKeys::CALLBACK_QUERY_KEY;
using Utils::TGBotApi::JSONKeys::CALLBACK_QUERY_ID_KEY;
using Utils::TGBotApi::JSONKeys::REPLY_MARKUP_KEY;
using Utils::TGBotApi::File::throw_if_not_correct_file;
using httplib::Params;
using std::to_string, std::map, std::move;

Bot::Bot(string_view token, const json& json_bot):
    _user(json_bot),
    _token(token),
    _secret_token(UUID4::generate_str()),
    _can_join_groups(json_bot[CAN_JOIN_GROUPS_KEY]),
    _can_read_all_group_messages(json_bot[CAN_READ_ALL_GROUP_MESSAGES_KEY]),
    _supports_inline_queries(json_bot[SUPPORTS_INLINE_QUERIES_KEY]),
    _can_connect_to_business(json_bot[CAN_CONNECT_TO_BUSINESS_KEY]),
    _has_main_web_app(json_bot[HAS_MAIN_WEB_APP_KEY]) {}

Bot::Bot(string_view token): 
Bot(token, _get_me_raw_json(token)) {}

const json Bot::_get_me_raw_json(string_view token) { 
    Query client(token);
    const json result = client.query_raw_json(EnumQueryMethod::GET, "getMe")[RESULT_KEY];
    return result;
}

long long Bot::get_id() const noexcept {
    return _user.id;
}
const string& Bot::get_name() const noexcept {
    return _user.name;
}
const string& Bot::get_username() const noexcept {
    return _user.username;
}

bool Bot::can_join_groups() const noexcept { 
    return _can_join_groups; 
}
bool Bot::can_read_all_group_messages() const noexcept { 
    return _can_read_all_group_messages; 
}
bool Bot::supports_inline_queries() const noexcept { 
    return _supports_inline_queries; 
}
bool Bot::can_connect_to_business() const noexcept { 
    return _can_connect_to_business; 
}
bool Bot::has_main_web_app() const noexcept { 
    return _has_main_web_app; 
}

bool Bot::delete_webhook() const { 
    return *Query(_token).query_parse_json<bool>(
        EnumQueryMethod::GET, 
        "deleteWebhook"
    ).result; 
}

bool Bot::set_webhook(string_view webhook_url) const { 
    return *Query(_token).query_parse_json<bool>(
        EnumQueryMethod::POST,
        "setWebhook", 
        Params{
            {URL_KEY, string(webhook_url)},
            {SECRET_TOKEN_KEY, _secret_token},
            {ALLOWED_UPDATES_KEY, json{MESSAGE_KEY, CALLBACK_QUERY_KEY}.dump()}
        }
    ).result; 
}

unique_ptr<Message> Bot::send_message(const SendMessageParameters& message_parameters) const {
    string path = map<EnumContentType, string>{
        {EnumContentType::TEXT, "sendMessage"},
        {EnumContentType::DOCUMENT, "sendDocument"},
        {EnumContentType::VIDEO, "sendVideo"},
        {EnumContentType::PHOTO, "sendPhoto"},
        {EnumContentType::AUDIO, "sendAudio"},
    }[message_parameters.content_type];

    Params params;
    Query::Files files;
    params.insert({CHAT_ID_KEY, to_string(message_parameters.chat_id)});
    params.insert({PARSE_MODE_KEY, message_parameters.parse_mode});
    if (!message_parameters.text.empty()) {
        params.insert({
            (
                message_parameters.content_type == EnumContentType::TEXT
                ? TEXT_KEY
                : CAPTION_KEY
            ),
            message_parameters.text
        });
    }

    if (message_parameters.reply_message_id != 0) {
        params.insert({REPLY_TO_MESSAGE_ID_KEY, to_string(message_parameters.reply_message_id)});
    }

    if (!message_parameters.placeholder.empty()) {
        params.insert({PLACEHOLDER_KEY, message_parameters.placeholder});
    }

    if (!message_parameters.filepath.empty() && message_parameters.content_type != EnumContentType::TEXT) {
        files.push_back(Query::File{
            .name = map<EnumContentType, string>{
                {EnumContentType::DOCUMENT, DOCUMENT_KEY},
                {EnumContentType::VIDEO, VIDEO_KEY},
                {EnumContentType::PHOTO, PHOTO_KEY},
                {EnumContentType::AUDIO, AUDIO_KEY},
            }[message_parameters.content_type],
            .filepath = message_parameters.filepath,
            .filename = message_parameters.filename.empty() ? message_parameters.filepath : message_parameters.filename,
            .content_type = DOCUMENT_KEY
        });
    }

    if (message_parameters.inline_keyboard != nullptr) {
        params.insert({REPLY_MARKUP_KEY, message_parameters.inline_keyboard->get_json()});
    }

    if (message_parameters.reply_keyboard != nullptr) {
        params.insert({REPLY_MARKUP_KEY, message_parameters.reply_keyboard->get_json()});
    }

    return move(Query(_token).query_parse_json<Message>(
        EnumQueryMethod::POST,
        path,
        params,
        {},
        files
    ).result);
}

unique_ptr<Message> Bot::edit_text(long long chat_id, long long message_id, string_view text) const {
    return move(Query(_token).query_parse_json<Message>(
        EnumQueryMethod::POST,
        "editMessageText",
        Params{
            {CHAT_ID_KEY, to_string(chat_id)},
            {MESSAGE_ID_KEY, to_string(message_id)},
            {TEXT_KEY, string(text)},
        }
    ).result);

}

unique_ptr<Message> Bot::edit_caption(long long chat_id, long long message_id, string_view caption) const {
    return move(Query(_token).query_parse_json<Message>(
        EnumQueryMethod::POST,
        "editMessageCaption",
        Params{
            {CHAT_ID_KEY, to_string(chat_id)},
            {MESSAGE_ID_KEY, to_string(message_id)},
            {CAPTION_KEY, string(caption)},
        }
    ).result);
}

bool Bot::delete_message(long long chat_id, long long message_id) const {
    return *Query(_token).query_parse_json<bool>(
        EnumQueryMethod::POST,
        "deleteMessage",
        Params{
            {CHAT_ID_KEY, to_string(chat_id)},
            {MESSAGE_ID_KEY, to_string(message_id)},
        }
    ).result;
}

bool Bot::answer_callback_query(
    string_view callback_query_id, 
    string_view text,
    bool show_alert
) const {
    Params params{
        {CALLBACK_QUERY_ID_KEY, string(callback_query_id)}
    };

    if (!text.empty()) {
        params.insert({TEXT_KEY, string(text)});
    }

    if (show_alert) {
        params.insert({SHOW_ALERT_KEY, TRUE_KEY});
    }

    return *Query(_token).query_parse_json<bool>(
        EnumQueryMethod::POST,
        "answerCallbackQuery",
        params
    ).result;
}

bool Bot::check_secret_token(string_view secret_token) const noexcept {
    return _secret_token == secret_token;
}

}
