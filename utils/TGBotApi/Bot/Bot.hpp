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

    struct Bot final : InterfaceBot {

        explicit Bot(string_view token, string_view telegram_api_url = "https://api.telegram.org");

        long long get_id() const noexcept override;
        const string& get_name() const noexcept override;
        const string& get_username() const noexcept override;
        bool can_join_groups() const noexcept override;
        bool can_read_all_group_messages() const noexcept override;
        bool supports_inline_queries() const noexcept override;
        bool can_connect_to_business() const noexcept override;
        bool has_main_web_app() const noexcept override;
        unique_ptr<Message> send_message(const SendMessageParameters& message_parameters) const override;
        unique_ptr<Message> edit_text(long long chat_id, long long message_id, string_view text) const override;
        unique_ptr<Message> edit_caption(long long chat_id, long long message_id, string_view caption) const override;
        bool delete_message(long long chat_id, long long message_id) const override;
        bool delete_webhook() const override;
        bool set_webhook(string_view) const override;
        bool answer_callback_query(string_view callback_query_id, string_view text, bool show_alert) const override;
        bool check_secret_token(string_view secret_token) const noexcept override;
        bool download_file(string_view file_id, string_view output_path) const override;

        private:

        explicit Bot(string_view token, string_view telegram_api_url, const json& json_bot);
        static json _get_me_raw_json(string_view token, string_view telegram_api_url);

        User _user;
        string _token;
        string _telegram_api_url;
        string _secret_token;
        bool _can_join_groups;
        bool _can_read_all_group_messages;
        bool _supports_inline_queries;
        bool _can_connect_to_business;
        bool _has_main_web_app;
    };

}
