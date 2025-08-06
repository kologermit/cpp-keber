#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <utils/TGBotApi/User/User.hpp>
#include <vector>
#include <httplib/httplib.h>

namespace Utils::TGBotApi::Bot {

using Utils::TGBotApi::User::User;
using nlohmann::json;
using std::vector;
using std::pair;
using std::nullopt;
using httplib::Params;

struct Bot : InterfaceBot {
    
    Bot(string_view token);

    long long           get_id()                                                                       const noexcept override;
    const string&       get_name()                                                                     const noexcept override;
    const string&       get_username()                                                                 const noexcept override;
    bool                can_join_groups()                                                              const noexcept override;
    bool                can_read_all_group_messages()                                                  const noexcept override;
    bool                supports_inline_queries()                                                      const noexcept override;
    bool                can_connect_to_business()                                                      const noexcept override;
    bool                has_main_web_app()                                                             const noexcept override;
    unique_ptr<Message> send_message(const SendMessageParameters& message_parameters)                           const override;
    unique_ptr<Message> edit_text(long long chat_id, long long message_id, string_view text)                    const override;
    unique_ptr<Message> edit_caption(long long chat_id, long long message_id, string_view caption)              const override;
    bool                delete_message(long long chat_id, long long message_id)                                 const override;
    bool                delete_webhook()                                                                        const override;
    bool                set_webhook(string_view)                                                                const override;
    bool                answer_callback_query(string_view callback_query_id, string_view text, bool show_alert) const override;
    bool                check_secret_token(string_view secret_token)                                   const noexcept override;

    private:

    explicit Bot(string_view, const json&);
    static const json _get_me_raw_json(string_view token);

    User _user;
    string _token;
    string _secret_token;
    bool   _can_join_groups;
    bool   _can_read_all_group_messages;
    bool   _supports_inline_queries;
    bool   _can_connect_to_business;
    bool   _has_main_web_app;
};

}
