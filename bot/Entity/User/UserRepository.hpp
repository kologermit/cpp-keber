#pragma once

#include <bot/Entity/User/InterfaceUserRepository.hpp>
#include <pqxx/pqxx>

namespace Bot::Entity::User {

    struct UserRepository final : InterfaceUserRepository {
        unique_ptr<User> get_by_telegram_user(const TGUser& tg_user) override;
        unique_ptr<User> get_by_username(string_view username) override;

        explicit UserRepository(connection& db);
    };

}