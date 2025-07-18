#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <utils/TGBotApi/User/User.hpp>
#include <vector>
#include <httplib/httplib.h>

namespace Utils::TGBotApi::Bot {

using Utils::TGBotApi::User::User;
using Utils::Types::optional_const_string;
using nlohmann::json;
using std::vector;
using std::pair;
using std::nullopt;
using httplib::Params;

struct Bot : virtual InterfaceBot, User {
    
    Bot(string_view);

    bool can_join_groups() const noexcept override;
    bool can_read_all_group_messages() const noexcept override;
    bool supports_inline_queries() const noexcept override;
    bool can_connect_to_business() const noexcept override;
    bool has_main_web_app() const noexcept override;


    void delete_webhook() const override;
    void set_webhook(string_view) const override;

    ptrMessage send_message(const SendMessageParameters& message_parameters) const override;

    private:

    explicit Bot(string_view, const json&);

    static const json _get_me_raw_json(string_view token);

    const_string _token;
    const_string _secret;
    bool _can_join_groups;
    bool _can_read_all_group_messages;
    bool _supports_inline_queries;
    bool _can_connect_to_business;
    bool _has_main_web_app;
};

}
