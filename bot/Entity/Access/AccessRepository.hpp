#pragma once

#include <bot/Entity/Access/InterfaceAccessRepository.hpp>
#include <pqxx/pqxx>

namespace Bot::Entity::Access {

using pqxx::connection;

struct AccessRepository : InterfaceAccessRepository {
    UserAccess get_by_user_id(int user_id) override;
    unique_ptr<Access> create(const Access& access) override;
    unique_ptr<Access> update(const Access& access, vector<int> columns) override;
    unique_ptr<Access> del(int id) override;

    AccessRepository(connection& db);

    private:

    connection& _db;
};

}