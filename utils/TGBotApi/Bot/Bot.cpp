#include <utils/UUID4/UUID4.cpp>
#include <utils/TGBotApi/Query/Query.cpp>
#include <utils/TGBotApi/Message/Message.cpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/TGBotApi/Bot/Bot.hpp>
#include <utils/TGBotApi/File/File.hpp>
#include <iostream>
#include <httplib.h>

namespace Utils::TGBotApi::Bot {

using Utils::TGBotApi::Query::Query;
using Utils::TGBotApi::Query::EnumQueryMethod;
using Utils::TGBotApi::Message::Message;
using Utils::TGBotApi::JSONKeys::RESULT_KEY;
using Utils::TGBotApi::JSONKeys::SECRET_KEY;
using Utils::TGBotApi::JSONKeys::CHAT_ID_KEY;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;
using Utils::TGBotApi::JSONKeys::URL_KEY;
using Utils::TGBotApi::JSONKeys::CAN_JOIN_GROUPS_KEY;
using Utils::TGBotApi::JSONKeys::CAN_READ_ALL_GROUP_MESSAGES_KEY;
using Utils::TGBotApi::JSONKeys::SUPPORTS_INLINE_QUERIES_KEY;
using Utils::TGBotApi::JSONKeys::CAN_CONNECT_TO_BUSINESS_KEY;
using Utils::TGBotApi::JSONKeys::HAS_MAIN_WEB_APP_KEY;
using Utils::TGBotApi::JSONKeys::REPLY_TO_MESSAGE_ID_KEY;
using Utils::TGBotApi::JSONKeys::CAPTION_KEY;
using Utils::TGBotApi::File::throw_if_not_correct_file;
using httplib::Params;
using std::make_shared;
using std::make_pair;
using std::to_string;

Bot::Bot(string_view token, const json& json_bot):
    User(json_bot),
    _token(token),
    _secret(UUID4::generate_str()),
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

const Params Bot::_get_params_with_optional(
    vector<pair<const_string, optional_const_string> > raw_params
) {
    Params params;
    for (auto raw_param : raw_params) {
        if (raw_param.second.has_value()) {
            params.insert({raw_param.first, *raw_param.second});
        }
    }
    return params;
}

bool Bot::can_join_groups() const noexcept { return _can_join_groups; }
bool Bot::can_read_all_group_messages() const noexcept { return _can_read_all_group_messages; }
bool Bot::supports_inline_queries() const noexcept { return _supports_inline_queries; }
bool Bot::can_connect_to_business() const noexcept { return _can_connect_to_business; }
bool Bot::has_main_web_app() const noexcept { return _has_main_web_app; }

void Bot::delete_webhook() const { 
    Query(_token).query(EnumQueryMethod::GET, "deleteWebhook"); 
}

void Bot::set_webhook(string_view webhook_url) const { 
    Query(_token).query(
        EnumQueryMethod::POST,
        "setWebhook", 
        Params{
            {URL_KEY, const_string(webhook_url)},
            {SECRET_KEY, _secret}
        }
    ); 
}

Bot::ptrMessage Bot::send_text(int chat_id, string_view text, optional_string_view reply_message_id) const {
    return Query(_token).query_parse_json<Message>(
        EnumQueryMethod::POST,
        "sendMessage",
        _get_params_with_optional(
            {
                {CHAT_ID_KEY, optional_const_string(to_string(chat_id))},
                {TEXT_KEY, optional_const_string(const_string(text))},
                {REPLY_TO_MESSAGE_ID_KEY, optional_const_string(reply_message_id)}
            }
        )
    ).result;
}

Bot::ptrMessage Bot::send_photo(int chat_id, string_view filepath, optional_string_view text, optional_string_view reply_message_id) const {
    throw_if_not_correct_file(filepath);
    return Query(_token).query_parse_json<Message>(
        EnumQueryMethod::POST,
        "sendPhoto",
        _get_params_with_optional(
            {
                {CHAT_ID_KEY, optional_const_string(to_string(chat_id))},
                {CAPTION_KEY, optional_const_string(text)},
                {REPLY_TO_MESSAGE_ID_KEY, optional_const_string(reply_message_id)}
            }
        ),
        nullopt,
        Query::Files{
            Query::File{
                .name = "photo",
                .filepath = const_string(filepath),
                .filename = const_string(filepath),
                .content_type = "image/jpeg"
            }
        }
    ).result;
}

Bot::ptrMessage Bot::send_audio(int chat_id, string_view filepath, optional_string_view text, optional_string_view reply_message_id) const {
    throw_if_not_correct_file(filepath);
    return Query(_token).query_parse_json<Message>(
        EnumQueryMethod::POST,
        "sendAudio",
        _get_params_with_optional(
            {
                {CHAT_ID_KEY, optional_const_string(to_string(chat_id))},
                {CAPTION_KEY, optional_const_string(text)},
                {REPLY_TO_MESSAGE_ID_KEY, optional_const_string(reply_message_id)}
            }
        ),
        nullopt,
        Query::Files{
            Query::File{
                .name = "audio",
                .filepath = const_string(filepath),
                .filename = const_string(filepath),
                .content_type = "audio/mp3"
            }
        }
    ).result;
}

Bot::ptrMessage Bot::send_video(int chat_id, string_view filepath, optional_string_view text, optional_string_view reply_message_id) const {
    throw_if_not_correct_file(filepath);
    return Query(_token).query_parse_json<Message>(
        EnumQueryMethod::POST,
        "sendVideo",
        _get_params_with_optional(
            {
                {CHAT_ID_KEY, optional_const_string(to_string(chat_id))},
                {CAPTION_KEY, optional_const_string(text)},
                {REPLY_TO_MESSAGE_ID_KEY, optional_const_string(reply_message_id)}
            }
        ),
        nullopt,
        Query::Files{
            Query::File{
                .name = "video",
                .filepath = const_string(filepath),
                .filename = const_string(filepath),
                .content_type = "video/mp4"
            }
        }
    ).result;
}

Bot::ptrMessage Bot::send_document(
    int chat_id, 
    string_view filepath, 
    optional_string_view filename, 
    optional_string_view text,
    optional_string_view reply_message_id
) const {
    throw_if_not_correct_file(filepath);
    return Query(_token).query_parse_json<Message>(
        EnumQueryMethod::POST,
        "sendDocument",
        _get_params_with_optional(
            {
                {CHAT_ID_KEY, optional_const_string(to_string(chat_id))},
                {CAPTION_KEY, optional_const_string(text)},
                {REPLY_TO_MESSAGE_ID_KEY, optional_const_string(reply_message_id)}
            }
        ),
        nullopt,
        Query::Files{
            Query::File{
                .name = "document",
                .filepath = const_string(filepath),
                .filename = const_string(filename.value_or(filepath)),
                .content_type = "document"
            }
        }
    ).result;
}

}
