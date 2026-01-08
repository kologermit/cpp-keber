#pragma once

#include <utils/BotApi/User.hpp>
#include <utils/BotApi/ContentType.hpp>
#include <utils/BotApi/InlineButton.hpp>
#include <string_view>
#include <vector>
#include <memory>

namespace Utils::BotApi {
    using std::optional;
    using std::unique_ptr;
    using std::string_view;
    using std::vector;

    struct InterfaceBotApi {
        virtual ~InterfaceBotApi() = default;

        virtual unique_ptr<User> get_user(long long id) = 0;
        virtual long long post_message(
            optional<long long> chat_telegram_id,
            string_view text,
            string_view file_name = "",
            ContentType content_type = ContentType::UNKNOWN,
            long long reply_to_message_id = 0,
            const vector<vector<InlineButton>>& = {},
            const vector<vector<string>>& = {}
        ) = 0;
        virtual long long patch_message(
            long long chat_telegram_id,
            long long message_id,
            string_view text,
            bool is_caption = false
        ) = 0;
    };
}