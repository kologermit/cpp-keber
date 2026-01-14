#pragma once

#include <utils/Datetime.hpp>
#include <nlohmann/json.hpp>

namespace Utils::Api {
    using std::optional;
    using std::nullopt;
    using std::string;
    using nlohmann::json;
    using jed_utils::datetime;
    using Utils::Datetime::DATETIME_FORMAT;

    constexpr const char* ID_KEY = "id";
    constexpr const char* CREATED_AT_KEY = "created_at";
    constexpr const char* UPDATED_AT_KEY = "updated_at";
    constexpr const char* DELETED_AT_KEY = "deleted_at";

    struct Entity {
        long long id;
        datetime created_at;
        datetime updated_at;
        optional<datetime> deleted_at;

        Entity(const json& json_entity):
            id(json_entity[ID_KEY].get<long long>()),
            created_at(datetime::parse(DATETIME_FORMAT, json_entity[CREATED_AT_KEY].get<string>())),
            updated_at(datetime::parse(DATETIME_FORMAT, json_entity[UPDATED_AT_KEY].get<string>())),
            deleted_at(
                json_entity[DELETED_AT_KEY].is_null()
                ? nullopt
                : optional<datetime>(datetime::parse(DATETIME_FORMAT, json_entity[DELETED_AT_KEY].get<string>()))
            )
        {}
    };
}