#pragma once

#include <bot/Entity/Callback/Callback.hpp>
#include <utils/TGBotApi/CallbackQuery/CallbackQuery.hpp>
#include <utils/Entity/Repository.hpp>
#include <memory>

namespace Bot::Entity::Callback {

    using TGCallback = Utils::TGBotApi::CallbackQuery::CallbackQuery;
    using Utils::Entity::Repository;
    using pqxx::connection;
    using std::unique_ptr;

    struct InterfaceCallbackRepository : Repository<Callback> {
        explicit InterfaceCallbackRepository(connection& db): Repository(db) {}
        virtual unique_ptr<Callback> get_by_telegram_callback(const TGCallback& tg_callback) = 0;
    };
}