#pragma once

#include <bot/Entity/Callback/InterfaceCallbackRepository.hpp>

namespace Bot::Entity::Callback {

    using pqxx::connection;

    struct CallbackRepository final : InterfaceCallbackRepository {
        CallbackRepository(connection& db): _db(db) {};

        unique_ptr<Callback> get_by_id(int id) override;
        unique_ptr<Callback> get_by_telegram_id(string_view telegram_id) override;
        unique_ptr<Callback> get_by_telegram_callback(
            const TGCallback& tg_callback,
            int message_id,
            int user_id,
            int chat_id
        ) override;
        unique_ptr<Callback> create(const Callback& callback) override;

    private:
        connection& _db;
    };
}