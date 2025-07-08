#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <utils/Logger/InterfaceLogger.cpp>
#include <utils/Type.cpp>
#include <utils/UUID4/UUID4.cpp>
#include <utils/TGBotApi/Query.cpp>
#include <utils/TGBotApi/User/User.cpp>
#include <utils/TGBotApi/Bot/InterfaceBot.cpp>
#include <utils/TGBotApi/Message/InterfaceMessage.cpp>
#include <utils/TGBotApi/Message/Message.cpp>
#include <utils/TGBotApi/JSONKeys.cpp>

namespace Utils::TGBotApi::Bot {

using Utils::Type::const_string;
using Utils::TGBotApi::User::User;
using Utils::TGBotApi::Query::Query;
using Utils::TGBotApi::Query::EnumQueryMethod;
using Utils::TGBotApi::Message::InterfaceMessage;
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
using nlohmann::json;
using std::string_view, std::make_shared, std::to_string;

struct Bot : virtual InterfaceBot, User {
    
    Bot(string_view);

    bool can_join_groups() const noexcept override;
    bool can_read_all_group_messages() const noexcept override;
    bool supports_inline_queries() const noexcept override;
    bool can_connect_to_business() const noexcept override;
    bool has_main_web_app() const noexcept override;


    void delete_webhook() const override;
    void set_webhook(string_view) const override;
    ptrMessage send_text(int, string_view) const override;

    protected:

    Bot(string_view, const json&);

    static const json get_me_raw_json(string_view token) { 
        return Query(token).query_raw_json(EnumQueryMethod::GET, "getMe")[RESULT_KEY];
    }

    const_string _token;
    const_string _secret;
    bool _can_join_groups;
    bool _can_read_all_group_messages;
    bool _supports_inline_queries;
    bool _can_connect_to_business;
    bool _has_main_web_app;
};

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
Bot(token, get_me_raw_json(token)) {}

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
        EnumQueryMethod::GET,
        "setWebhook", 
        {
            {URL_KEY, const_string(webhook_url)},
            {SECRET_KEY, _secret}
        }
    ); 
}

Bot::ptrMessage Bot::send_text(int chat_id, string_view text) const {
    return make_shared<Message>(
        Query(_token).query_raw_json(
            EnumQueryMethod::GET,
            "sendMessage",
            {
                {CHAT_ID_KEY, to_string(chat_id)},
                {TEXT_KEY, const_string(text)}
            }
        )[RESULT_KEY]
    );
}

}