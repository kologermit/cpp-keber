#pragma once

#include <bot/Entity/User/InterfaceUserRepository.hpp>
#include <pqxx/pqxx>

namespace Bot::Entity::User {

using pqxx::connection;

struct UserRepository : InterfaceUserRepository {

    unique_ptr<User> get_by_id(int id) override;
    unique_ptr<User> get_by_telegram_id(long long telegram_id) override;
    unique_ptr<User> get_by_telegram_user(const TGUser& tg_user) override;
    unique_ptr<User> create(const User& user) override;
    unique_ptr<User> update(const User& user, vector<int> columns) override;
    unique_ptr<User> del(int id) override;

    UserRepository(string_view conn_url);

    private:

    connection _db;
};

}