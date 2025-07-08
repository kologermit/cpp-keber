#pragma once

#include <utils/TGBotApi/Bot/InterfaceBot.hpp>

namespace Utils::TGBotApi::Bot {

using Utils::TGBotApi::User::User;
using nlohmann::json;

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

    static const json get_me_raw_json(string_view token);

    const_string _token;
    const_string _secret;
    bool _can_join_groups;
    bool _can_read_all_group_messages;
    bool _supports_inline_queries;
    bool _can_connect_to_business;
    bool _has_main_web_app;
};

}
