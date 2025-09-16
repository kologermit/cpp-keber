#pragma once

#include <bot/Entity/User/InterfaceUserRepository.hpp>
#include <utils/Entity/Repository.hpp>
#include <pqxx/pqxx>

namespace Bot::Entity::User {

    using pqxx::connection;
    using Utils::Entity::Repository;

    struct UserRepository final : InterfaceUserRepository, Repository<User> {

        unique_ptr<User> get_by_id(int id) override;
        unique_ptr<User> get_by_telegram_user(const TGUser& tg_user) override;
        unique_ptr<User> get_by_username(string_view username) override;
        unique_ptr<User> create(const User& user) override;
        unique_ptr<User> update(const User& user) override;
        unique_ptr<User> del(int id, bool is_soft) override;

        UserRepository(connection& db);

    };

}