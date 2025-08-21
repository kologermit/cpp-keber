#pragma once

#include <utils/Entity/Entity.hpp>

namespace Bot::Entity::Access {

using Utils::Entity::Entity;
using std::string;
using std::string_view;
using std::optional;
using std::to_string;
using std::nullopt;
using std::map;
using pqxx::row;
using jed_utils::datetime;

enum EnumAccessType {
    FULL,
    ACCESS,
    YOUTUBE,
    TASK,
    DOCKER,
    SERVER,
};

const map<int, string> map_access_type_to_string{
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(FULL),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(ACCESS),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(YOUTUBE),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(TASK),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(DOCKER),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(SERVER),
};

enum AccessColumns {
    USER_ID,
    TYPE,
};

constexpr const char* ACCESSES_TABLE = "\"accesses\"";
constexpr const char* ACCESS_TYPES_TABLE = "\"access_types\"";
constexpr const char* USER_ID_COLUMN = "\"user_id\"";
constexpr const char* TYPE_COLUMN = "\"type\"";

struct Access : Entity {

    int user_id;
    EnumAccessType type;

    Access(
        int id = 0, 
        optional<datetime> created_at = nullopt, 
        optional<datetime> updated_at = nullopt, 
        optional<datetime> deleted_at = nullopt,
        int user_id = 0,
        EnumAccessType type = FULL
    ):
    Entity(id, created_at, updated_at, deleted_at),
    user_id(user_id),
    type(type) {}

    Access(const row& access_row):
    Entity(access_row),
    user_id(access_row[USER_ID_COLUMN].get<int>().value()),
    type(static_cast<EnumAccessType>(access_row[TYPE_COLUMN].get<int>().value())) {}

    string get_data_by_column(int column) const {
        switch (column)
        {
        case USER_ID:
            return to_string(user_id);
            break;
        case TYPE:
            return to_string(type);
            break; 
        default:
            return "";
            break;
        }
    }

};

}