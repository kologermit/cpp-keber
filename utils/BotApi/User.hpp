#pragma once

#include <nlohmann/json.hpp>
#include <datetime/datetime.h>
#include <string>
#include <optional>
#include <utils/Datetime.hpp>

namespace Utils::BotApi {
    using std::optional;
    using std::nullopt;
    using std::string;
    using jed_utils::datetime;
    using nlohmann::json;
    using Utils::Datetime::DATETIME_FORMAT;

    struct User {
        const long long id;
        const string name;
        const string username;
        const datetime created_at;
        const datetime updated_at;
        const optional<datetime> deleted_at;

        User(const json& json_user):
            id(json_user["id"].get<long long>()),
            name(json_user["name"].get<string>()),
            username(json_user["username"].get<string>()),
            created_at(datetime::parse(DATETIME_FORMAT, json_user["created_at"].get<string>())),
            updated_at(datetime::parse(DATETIME_FORMAT, json_user["updated_at"].get<string>())),
            deleted_at(
                json_user["deleted_at"].is_null()
                ? nullopt
                : optional<datetime>(datetime::parse(DATETIME_FORMAT, json_user["deleted_at"].get<string>()))
            )
        {}
    };
}