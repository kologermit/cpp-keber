#include <bot/Entity/Access/AccessRepository.hpp>
#include <utils/Entity/exec_templates.hpp>

namespace Bot::Entity::Access {

    using Utils::Entity::create_rows_in_enum_table_if_empty;
    using Utils::Entity::exec_select_many;
    using std::map;

    AccessRepository::AccessRepository(connection& db):
    InterfaceAccessRepository(db) {
        create_rows_in_enum_table_if_empty(_db, ACCESS_TYPES_TABLE, map_access_type_to_string);
    }
    
    vector<unique_ptr<Access> > AccessRepository::get_raw_by_user_id(long long user_id) {
        return exec_select_many<Access>(_db, Access::get_table_name(), {{USER_ID->name, to_string(user_id)}});
    }


    UserAccess AccessRepository::get_by_user_id(long long user_id) {
        UserAccess user_access;
        
        const map<EnumAccessType, bool&> type_to_member = {
            {BASE, user_access.base},
            {FULL, user_access.full},
            {ACCESS, user_access.access},
            {YOUTUBE, user_access.youtube},
            {TASK, user_access.task},
            {DOCKER, user_access.docker},
            {SERVER, user_access.server}
        };

        for (auto& access : get_raw_by_user_id(user_id)) {
            type_to_member.at(access->type) = true;
        }
        
        return user_access;
    }
}