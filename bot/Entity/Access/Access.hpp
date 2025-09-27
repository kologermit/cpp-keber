#pragma once

#include <utils/Entity/Entity.hpp>

namespace Bot::Entity::Access {

    using Utils::Entity::Entity;
    using Utils::Entity::Column;
    using std::make_shared;
    using std::string;
    using std::string_view;
    using std::optional;
    using std::to_string;
    using std::nullopt;
    using std::map;
    using pqxx::row;
    using jed_utils::datetime;

    enum EnumAccessType {
        BASE,
        FULL,
        ACCESS,
        YOUTUBE,
        TASK,
        DOCKER,
        SERVER,
    };

    const map<int, string> map_access_type_to_string{
        {EnumAccessType::BASE, "BASE"},
        {EnumAccessType::FULL, "FULL"},
        {EnumAccessType::ACCESS, "ACCESS"},
        {EnumAccessType::YOUTUBE, "YOUTUBE"},
        {EnumAccessType::TASK, "TASK"},
        {EnumAccessType::DOCKER, "DOCKER"},
        {EnumAccessType::SERVER, "SERVER"},
    };

    constexpr const char* ACCESS_TYPES_TABLE = "access_types";
    const auto USER_ID = make_shared<Column>("user_id");
    const auto TYPE = make_shared<Column>("type");

    struct UserAccess {
        bool base = false;
        bool full = false;
        bool access = false;
        bool youtube = false;
        bool task = false;
        bool docker = false;
        bool server = false;
    };

    struct Access : Entity {

        long long user_id;
        EnumAccessType type;

        Access(
            long long user_id = 0,
            EnumAccessType type = FULL,
            long long id = 0, 
            datetime created_at = {}, 
            datetime updated_at = {}, 
            optional<datetime> deleted_at = nullopt
        ):
        Entity(id, created_at, updated_at, deleted_at),
        user_id(user_id),
        type(type) {}

        Access(const row& access_row):
            Entity(access_row),
            user_id(access_row[USER_ID->name].get<long long>().value()),
            type(static_cast<EnumAccessType>(access_row[TYPE->name].get<int>().value())) 
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "accesses";
            return table;
        }

        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);

            result[USER_ID->name] = to_string(user_id);
            result[TYPE->name] = to_string(type);

            return result;
        }

    };
}