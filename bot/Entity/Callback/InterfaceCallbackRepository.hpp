#pragma once

#include <bot/Entity/Callback/Callback.hpp>
#include <utils/TGBotApi/CallbackQuery/CallbackQuery.hpp>
#include <memory>

namespace Bot::Entity::Callback {

    using TGCallback = Utils::TGBotApi::CallbackQuery::CallbackQuery;
    using std::unique_ptr;

    struct InterfaceCallbackRepository {
        virtual ~InterfaceCallbackRepository() = default;

        virtual unique_ptr<Callback> get_by_id(int id) = 0;
        virtual unique_ptr<Callback> get_by_telegram_id(string_view telegram_id) = 0;
        virtual unique_ptr<Callback> get_by_telegram_callback(
            const TGCallback& tg_callback,
            int message_id,
            int user_id,
            int chat_id
        ) = 0;
        virtual unique_ptr<Callback> create(const Callback& callback) = 0;

    };
}