#pragma once

#include <bot/Entity/Access/Access.hpp>
#include <utils/Entity/Repository.hpp>

namespace Bot::Entity::Access {

    using Utils::Entity::Repository;
    using pqxx::connection;
    using std::unique_ptr;
    using std::vector;

    struct InterfaceAccessRepository : Repository<Access> {
        InterfaceAccessRepository(connection& db): Repository(db) {}
        virtual vector<unique_ptr<Access> > get_raw_by_user_id(long long user_id) = 0;
        virtual UserAccess get_by_user_id(long long user_id) = 0;
    };
}