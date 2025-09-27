#pragma once

#include <bot/Entity/Access/InterfaceAccessRepository.hpp>
#include <pqxx/pqxx>

namespace Bot::Entity::Access {
    struct AccessRepository final : InterfaceAccessRepository {
        vector<unique_ptr<Access> > get_raw_by_user_id(long long user_id) override;
        UserAccess get_by_user_id(long long user_id) override;
        AccessRepository(connection& db);
    };

}