#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/BotApi/InterfaceBotApi.hpp>
#include <httplib/httplib.h>

namespace Utils::BotApi {
    using httplib::Client;

    struct BotApi final : InterfaceBotApi {
        BotApi(
            string_view base_url,
            string_view auth_key
        );

        unique_ptr<User> get_user(long long id) override;
        long long post_message(
            optional<long long> chat_telegram_id,
            string_view text,
            string_view file_name,
            ContentType content_type,
            long long reply_to_message_id,
            const vector<vector<InlineButton>>& inline_keyboard,
            const vector<vector<string>>& reply_markup
        ) override;
        long long patch_message(
            long long chat_telegram_id,
            long long message_id,
            string_view text,
            bool is_caption
        ) override;

        private:
        const string _base_url;
        const string _auth_key;
        Client _client;
    };
}