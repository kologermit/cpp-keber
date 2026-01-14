#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <utils/BotApi/InterfaceBotApi.hpp>
#include <utils/Api/Api.hpp>

namespace Utils::BotApi {
    using Utils::Api::Api;

    struct BotApi final : InterfaceBotApi, Api {
        BotApi(string_view base_url, string_view auth_key);
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
    };
}