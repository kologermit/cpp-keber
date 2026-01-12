#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/Datetime.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <map>

namespace TaskTracker::Entity::Task {
    using Utils::Entity::Entity;
    using Utils::Datetime::DATETIME_FORMAT;
    using std::optional;
    using std::nullopt;
    using std::string_view;
    using std::string;
    using std::to_string;
    using std::map;
    using std::make_shared;
    using std::shared_ptr;
    using jed_utils::datetime;
    using nlohmann::json;
    using pqxx::row;

    enum TaskState {UNKNOWN, NEW, IN_WORK, COMPLETED, DELETED};

    constexpr const char* TASK_STATES_TABLE = "task_states";
    constexpr const char* TITLE_COLUMN = "title";
    constexpr const char* DESCRIPTION_COLUMN = "description";
    constexpr const char* USER_ID_COLUMN = "user_id";
    constexpr const char* START_AT_COLUMN = "start_at";
    constexpr const char* IN_WORK_AT_COLUMN = "in_work_at";
    constexpr const char* COMPLETED_AT_COLUMN = "completed_at";
    constexpr const char* STATE_KEY = "state";

    struct Task : Entity {

        string title;
        string description;
        long long user_id;
        datetime start_at;
        optional<datetime> in_work_at;
        optional<datetime> completed_at;

        explicit Task(
            string_view title = "",
            string_view description = "",
            long long user_id = 0,
            datetime start_at = {},
            optional<datetime> in_work_at = nullopt,
            optional<datetime> completed_at = nullopt,
            long long id = 0,
            datetime created_at = {},
            datetime updated_at = {},
            optional<datetime> deleted_at = nullopt
        ):
            Entity(id, created_at, updated_at, deleted_at),
            title(title),
            description(description),
            user_id(user_id),
            start_at(start_at),
            in_work_at(in_work_at),
            completed_at(completed_at)
        {}

        explicit Task(const row& task_row):
            Entity(task_row),
            title(task_row[TITLE_COLUMN].get<string>().value()),
            description(task_row[DESCRIPTION_COLUMN].get<string>().value()),
            user_id(task_row[USER_ID_COLUMN].get<long long>().value()),
            start_at(datetime::parse(DATETIME_FORMAT, task_row[START_AT_COLUMN].get<string>().value())),
            in_work_at(
                task_row[IN_WORK_AT_COLUMN].is_null()
                ? nullopt
                : optional<datetime>(datetime::parse(DATETIME_FORMAT, task_row[IN_WORK_AT_COLUMN].get<string>().value()))
            ),
            completed_at(
                task_row[COMPLETED_AT_COLUMN].is_null()
                ? nullopt
                : optional<datetime>(datetime::parse(DATETIME_FORMAT, task_row[COMPLETED_AT_COLUMN].get<string>().value()))
            )
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "tasks";
            return table;
        }

        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);

            result[TITLE_COLUMN] = title;
            result[DESCRIPTION_COLUMN] = description;
            result[USER_ID_COLUMN] = to_string(user_id);
            result[START_AT_COLUMN] = start_at.to_string(DATETIME_FORMAT);
            if (in_work_at.has_value()) {
                result[IN_WORK_AT_COLUMN] = in_work_at->to_string();
            } else {
                result[IN_WORK_AT_COLUMN] = nullopt;
            }
            if (completed_at.has_value()) {
                result[COMPLETED_AT_COLUMN] = completed_at->to_string();
            } else {
                result[COMPLETED_AT_COLUMN] = nullopt;
            }

            return result;
        }

        json to_json() const noexcept {
            json result = Entity::to_json();
            result[TITLE_COLUMN] = title;
            result[DESCRIPTION_COLUMN] = description;
            result[USER_ID_COLUMN] = user_id;
            result[START_AT_COLUMN] = start_at.to_string(DATETIME_FORMAT);
            if (in_work_at.has_value()) {
                result[IN_WORK_AT_COLUMN] = in_work_at->to_string();
            } else {
                result[IN_WORK_AT_COLUMN] = nullptr;
            }
            if (completed_at.has_value()) {
                result[COMPLETED_AT_COLUMN] = completed_at->to_string();
            } else {
                result[COMPLETED_AT_COLUMN] = nullptr;
            }
            result[STATE_KEY] = get_state();

            return result;
        }

        TaskState get_state() const noexcept {
            if (deleted_at.has_value()) {
                return TaskState::DELETED;
            }
            if (completed_at.has_value()) {
                return TaskState::COMPLETED;
            }
            if (in_work_at.has_value()) {
                return TaskState::IN_WORK;
            }
            return TaskState::NEW;
        }
    };

}