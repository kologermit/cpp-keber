#pragma once

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

    ptrMessage send_text(int chat_id, string_view text, optional_string_view reply_message_id = nullopt) const override;
    ptrMessage send_photo(
        int chat_id, 
        string_view filepath, 
        optional_string_view text = nullopt,
        optional_string_view reply_message_id = nullopt
    ) const override;
    ptrMessage send_audio(
        int chat_id,
        string_view filepath,
        optional_string_view text = nullopt,
        optional_string_view reply_message_id = nullopt
    ) const override;
    ptrMessage send_video(
        int chat_id,
        string_view filepath,
        optional_string_view text = nullopt,
        optional_string_view reply_message_id = nullopt
    ) const override;
    ptrMessage send_document(
        int chat_id, 
        string_view filepath, 
        optional_string_view filename = nullopt, 
        optional_string_view text = nullopt,
        optional_string_view reply_message_id = nullopt
    ) const override;

    private:

    explicit Bot(string_view, const json&);

    static const json _get_me_raw_json(string_view token);
    static const Params _get_params_with_optional(
        vector<pair<const_string, optional_const_string> > raw_params
    );

    const_string _token;
    const_string _secret;
    bool _can_join_groups;
    bool _can_read_all_group_messages;
    bool _supports_inline_queries;
    bool _can_connect_to_business;
    bool _has_main_web_app;
};

}
