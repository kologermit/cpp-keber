#pragma once

#include <bot/Entity/User/User.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <utils/Entity/Repository.hpp>
#include <memory>
#include <vector>

namespace Bot::Entity::User {
    
    using std::unique_ptr;
    using std::vector;
    using pqxx::connection;
    using Utils::Entity::Repository;
    using TGUser = Utils::TGBotApi::Types::User;

    struct InterfaceUserRepository : Repository<User> {
        virtual unique_ptr<User> get_by_telegram_user(const TGUser& tg_user) = 0;
        virtual unique_ptr<User> get_by_username(string_view username) = 0;

        explicit InterfaceUserRepository(connection& db): Repository(db) {};
    };

}