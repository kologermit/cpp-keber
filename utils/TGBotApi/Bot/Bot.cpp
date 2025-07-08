#pragma once

#include <memory>
#include <utils/Logger/InterfaceLogger.cpp>
#include <utils/Type.cpp>
#include <utils/UUID4/UUID4.cpp>
#include <utils/TGBotApi/Query.cpp>
#include <utils/TGBotApi/User/User.cpp>
#include <utils/TGBotApi/Bot/InterfaceBot.cpp>
#include <utils/TGBotApi/JSONKeys.cpp>

namespace Utils {
namespace TGBotApi {
namespace Bot {

using namespace std;
using namespace fmt;
using namespace Utils::TGBotApi::User;
using namespace Utils::TGBotApi::JSONKeys;
using namespace Utils::Type;
using namespace Utils::Logger;

struct Bot : virtual InterfaceBot, User {
    
    Bot(const_string&);

    void delete_webhook() const override { 
        Query(_token).query(EnumQueryMethod::GET, "deleteWebhook"); 
    }
    void set_webhook(const_string& webhook_url) const override;
    bool get_can_join_groups() const override;
    bool get_can_read_all_group_messages() const override;
    bool get_supports_inline_queries() const override;
    bool get_can_connect_to_business() const override;
    bool get_has_main_web_app() const override;

    protected:
    struct BotDto : UserDto {
        bool can_join_groups;
        bool can_read_all_group_messages;
        bool supports_inline_queries;
        bool can_connect_to_business;
        bool has_main_web_app;

        BotDto(
            int id, 
            const_string& name, 
            const_string& username,
            bool can_join_groups,
            bool can_read_all_group_messages,
            bool supports_inline_queries,
            bool can_connect_to_business,
            bool has_main_web_app
        );
        BotDto(const InterfaceBot&);
        BotDto(const json&);
    };

    static BotDto get_me(const_string& token) { 
        return Query(token).query<BotDto>(EnumQueryMethod::GET, "getMe").result;
    }

    const_string _token;
    const_string _secret;
    bool _can_join_groups;
    bool _can_read_all_group_messages;
    bool _supports_inline_queries;
    bool _can_connect_to_business;
    bool _has_main_web_app;
};

Bot::Bot(const_string& token): 
    User(get_me(token)),
    _token(token), 
    _secret(UUID4::generate_str()) {
        auto bot_dto = get_me(token);
        _can_join_groups = bot_dto.can_join_groups;
        _can_read_all_group_messages = bot_dto.can_read_all_group_messages;
        _supports_inline_queries = bot_dto.supports_inline_queries;
        _can_connect_to_business = bot_dto.can_connect_to_business;
        _has_main_web_app = bot_dto.has_main_web_app;
    }

bool Bot::get_can_join_groups() const { return _can_join_groups; }
bool Bot::get_can_read_all_group_messages() const { return _can_read_all_group_messages; }
bool Bot::get_supports_inline_queries() const { return _supports_inline_queries; }
bool Bot::get_can_connect_to_business() const { return _can_connect_to_business; }
bool Bot::get_has_main_web_app() const { return _has_main_web_app; }

void Bot::set_webhook(const_string& webhook_url) const { 
    Query(_token).query(
        EnumQueryMethod::GET,
        "setWebhook", 
        {
            {URL_KEY, webhook_url},
            {SECRET_KEY, _secret}
        }
    ); 
}

Bot::BotDto::BotDto(
    int id = 0, 
    const_string& name = "", 
    const_string& username = "",
    bool can_join_groups = false,
    bool can_read_all_group_messages = false,
    bool supports_inline_queries = false,
    bool can_connect_to_business = false,
    bool has_main_web_app = false
    ):
    UserDto(id, name, username),
    can_join_groups(can_join_groups),
    can_read_all_group_messages(can_read_all_group_messages),
    supports_inline_queries(supports_inline_queries),
    can_connect_to_business(can_connect_to_business),
    has_main_web_app(has_main_web_app) {}

Bot::BotDto::BotDto(const InterfaceBot& bot):
    UserDto(bot),
    can_join_groups(bot.get_can_join_groups()),
    can_read_all_group_messages(bot.get_can_read_all_group_messages()),
    supports_inline_queries(bot.get_supports_inline_queries()),
    can_connect_to_business(bot.get_can_connect_to_business()),
    has_main_web_app(bot.get_has_main_web_app()) {}

Bot::BotDto::BotDto(const json& json_bot):
    UserDto(json_bot),
    can_join_groups(json_bot.value(CAN_JOIN_GROUPS_KEY, false)),
    can_read_all_group_messages(json_bot.value(CAN_READ_ALL_GROUP_MESSAGES_KEY, false)),
    supports_inline_queries(json_bot.value(SUPPORTS_INLINE_QUERIES_KEY, false)),
    can_connect_to_business(json_bot.value(CAN_CONNECT_TO_BUSINESS_KEY, false)),
    has_main_web_app(json_bot.value(HAS_MAIN_WEB_APP_KEY, false)) {}

}
}
}