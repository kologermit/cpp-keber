#pragma once

#include <bot/Entity/Callback/InterfaceCallbackRepository.hpp>

namespace Bot::Entity::Callback {

    using pqxx::connection;

    struct CallbackRepository final : InterfaceCallbackRepository {
        explicit CallbackRepository(connection& db): InterfaceCallbackRepository(db) {};
        unique_ptr<Callback> get_by_telegram_callback(const TGCallback& tg_callback) override;
    };
}