#include <bot/Entity/Access/AccessRepository.hpp>
#include <utils/Entity/exec_templates.hpp>

namespace Bot::Entity::Access {

using Utils::Entity::create_rows_in_enum_table_if_empty;
using Utils::Entity::exec_select_many;
using Utils::Entity::exec_select_one;
using Utils::Entity::exec_insert;
using Utils::Entity::exec_update_by_id;
using Utils::Entity::FailedUpdateException;
using Utils::Entity::DELETED_AT_COLUMN;
using Utils::Entity::DATETIME_FORMAT;
using std::map;

// debug
using Utils::Logger::get_logger;
using std::to_string;
// debug

AccessRepository::AccessRepository(connection& db):
_db(db) {
    create_rows_in_enum_table_if_empty(_db, ACCESS_TYPES_TABLE, map_access_type_to_string);
}
vector<unique_ptr<Access> > AccessRepository::get_raw_by_user_id(int user_id) {
    return exec_select_many<Access>(_db, ACCESSES_TABLE, {{USER_ID_COLUMN, to_string(user_id)}});
}


UserAccess AccessRepository::get_by_user_id(int user_id) {
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

    auto raw_accesses = get_raw_by_user_id(user_id);

    // debug
    get_logger()->debug("access::size", to_string(raw_accesses.size()), __FILE__, __LINE__);
    // debug
    
    for (auto& access : raw_accesses) {
        type_to_member.at(access->type) = true;
    }
    
    return user_access;
}

unique_ptr<Access> AccessRepository::create(const Access& access) {
    return exec_insert<Access>(_db, ACCESSES_TABLE, {
        {USER_ID_COLUMN, to_string(access.user_id)},
        {TYPE_COLUMN, to_string(access.type)}
    });
}

const string& throw_if_invalid_column_to_update(int column) {
    static const map<int, const string> map_column_to_name{
        {USER_ID, USER_ID_COLUMN},
        {TYPE, TYPE_COLUMN},
    };

    auto find_iterator = map_column_to_name.find(column);

    if (find_iterator == map_column_to_name.end()) {
        throw FailedUpdateException("column cant be updated");
    }

    return find_iterator->second;
}

unique_ptr<Access> AccessRepository::update(const Access& access, vector<int> columns) {
    map<string, string> data;

    for (int column : columns) {
        data[throw_if_invalid_column_to_update(column)] = access.get_data_by_column(column);
    }

    return exec_update_by_id<Access>(_db, ACCESSES_TABLE, data, access.id);
}


unique_ptr<Access> AccessRepository::del(int id) {
    return exec_update_by_id<Access>(_db, ACCESSES_TABLE, {{DELETED_AT_COLUMN, datetime().to_string(DATETIME_FORMAT)}}, id);
}

}