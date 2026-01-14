#pragma once

#include <utils/Api/Entity.hpp>

namespace Utils::TaskTrackerApi {
    using std::optional;
    using std::nullopt;
    using std::string;
    using Utils::Datetime::DATETIME_FORMAT;
    using nlohmann::json;
    using jed_utils::datetime;
    using Utils::Api::Entity;

    enum TaskState {UNKNOWN, NEW, IN_WORK, COMPLETED, DELETED};

    constexpr const char* TITLE_KEY = "title";
    constexpr const char* DESCRIPTION_KEY = "description";
    constexpr const char* USER_ID_KEY = "user_id";
    constexpr const char* STATE_KEY = "state";
    constexpr const char* START_AT_KEY = "start_at";
    constexpr const char* IN_WORK_AT_KEY = "in_work_at";
    constexpr const char* COMPLETED_AT_KEY = "completed_at";

    struct Task : Entity {
        string title;
        string description;
        long long user_id;
        TaskState state;
        datetime start_at;
        optional<datetime> in_work_at;
        optional<datetime> completed_at;

        Task(const json& json_task):
            Entity(json_task),
            title(json_task[TITLE_KEY].get<string>()),
            description(json_task[DESCRIPTION_KEY].get<string>()),
            user_id(json_task[USER_ID_KEY].get<long long>()),
            state(static_cast<TaskState>(json_task[STATE_KEY].get<int>())),
            start_at(datetime::parse(DATETIME_FORMAT, json_task[START_AT_KEY].get<string>())),
            in_work_at(
                json_task[IN_WORK_AT_KEY].is_null()
                ? nullopt
                : optional<datetime>(datetime::parse(DATETIME_FORMAT, json_task[IN_WORK_AT_KEY].get<string>()))
            ),
            completed_at(
                json_task[COMPLETED_AT_KEY].is_null()
                ? nullopt
                : optional<datetime>(datetime::parse(DATETIME_FORMAT, json_task[IN_WORK_AT_KEY].get<string>()))
            )
        {}
    };
}