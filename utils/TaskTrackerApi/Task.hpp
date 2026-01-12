#pragma once

#include <nlohmann/json.hpp>
#include <utils/Datetime.hpp>

namespace Utils::TaskTracker {
    using std::optional;
    using std::nullopt;
    using std::string;
    using Utils::Datetime::DATETIME_FORMAT;
    using nlohmann::json;
    using jed_utils::datetime;

    enum TaskState {UNKNOWN, NEW, IN_WORK, CANCELLED, COMPLETED};

    constexpr const char* TITLE_KEY = "title";
    constexpr const char* DESCRIPTION_KEY = "description";
    constexpr const char* USER_ID_KEY = "user_id";
    constexpr const char* STATE_KEY = "state";
    constexpr const char* ID_KEY = "id";
    constexpr const char* CREATED_AT_KEY = "created_at";
    constexpr const char* UPDATED_AT_KEY = "updated_at";
    constexpr const char* DELETED_AT_KEY = "delete_at";

    struct Task {
        string title;
        string description;
        long long user_id;
        TaskState state;
        long long id;
        datetime created_at;
        datetime updated_at;
        optional<datetime> deleted_at;


        Task(const json& json_task):
            title(json_task[TITLE_KEY].get<string>()),
            description(json_task[DESCRIPTION_KEY].get<string>()),
            user_id(json_task[USER_ID_KEY].get<long long>()),
            state(static_cast<TaskState>(json_task[STATE_KEY].get<int>())),
            id(json_task[ID_KEY].get<long long>()),
            created_at(datetime::parse(DATETIME_FORMAT, json_task[CREATED_AT_KEY].get<string>())),
            updated_at(datetime::parse(DATETIME_FORMAT, json_task[UPDATED_AT_KEY].get<string>())),
            deleted_at(
                json_task[DELETED_AT_KEY].is_null()
                ? nullopt
                : optional<datetime>(datetime::parse(DATETIME_FORMAT, json_task[DELETED_AT_KEY].get<string>()))
            )
        {}
    };
}