#pragma once

#include <utils/Entity/Entity.hpp>

namespace Bot::Entity::Access {

    using Utils::Entity::Entity;
    using std::string;
    using std::optional;
    using std::to_string;
    using std::nullopt;
    using std::map;
    using pqxx::row;
    using jed_utils::datetime;

    enum AccessType {
        BASE,
        FULL,
        ACCESS,
        YOUTUBE,
        TASK_TRACKER,
        DOCKER,
        SERVER,
    };

    const map<int, string> map_access_type_to_string{
        {AccessType::BASE, "BASE"},
        {AccessType::FULL, "FULL"},
        {AccessType::ACCESS, "ACCESS"},
        {AccessType::YOUTUBE, "YOUTUBE"},
        {AccessType::TASK_TRACKER, "TASK_TRACKER"},
        {AccessType::DOCKER, "DOCKER"},
        {AccessType::SERVER, "SERVER"},
    };

    constexpr const char* ACCESS_TYPES_TABLE = "access_types";
    constexpr const char* USER_ID_COLUMN = "user_id";
    constexpr const char* TYPE_COLUMN = "type";

    struct UserAccess {
        bool base = false;
        bool full = false;
        bool access = false;
        bool youtube = false;
        bool task_tracker = false;
        bool docker = false;
        bool server = false;
    };

    struct Access : Entity {

        long long user_id;
        AccessType type;

        explicit Access(
            long long user_id = 0,
            AccessType type = FULL,
            long long id = 0, 
            datetime created_at = {}, 
            datetime updated_at = {}, 
            optional<datetime> deleted_at = nullopt
        ):
        Entity(id, created_at, updated_at, deleted_at),
        user_id(user_id),
        type(type) {}

        explicit Access(const row& access_row):
            Entity(access_row),
            user_id(access_row[USER_ID_COLUMN].get<long long>().value()),
            type(static_cast<AccessType>(access_row[TYPE_COLUMN].get<int>().value()))
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "accesses";
            return table;
        }

        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);

            result[USER_ID_COLUMN] = to_string(user_id);
            result[TYPE_COLUMN] = to_string(type);

            return result;
        }

    };
}