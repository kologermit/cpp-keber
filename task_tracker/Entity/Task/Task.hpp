#pragma once

#include <utils/Entity/Entity.hpp>
#include <string>
#include <map>

namespace TaskTracker::Entity::Task {
    using Utils::Entity::Column;
    using Utils::Entity::Entity;
    using std::optional;
    using std::nullopt;
    using std::string_view;
    using std::string;
    using std::to_string;
    using std::map;
    using std::make_shared;
    using std::shared_ptr;
    using jed_utils::datetime;
    using pqxx::row;

    constexpr const char* TASK_STATES_TABLE = "task_states";
    const shared_ptr<Column> TITLE = make_shared<Column>("title");
    const shared_ptr<Column> DESCRIPTION = make_shared<Column>("description");
    const shared_ptr<Column> USER_ID = make_shared<Column>("user_id");
    const shared_ptr<Column> STATE = make_shared<Column>("state");

    enum TaskState {UNKNOWN, NEW, IN_WORK, CANCELLED, COMPLETED};

    const map<int, string> map_task_state_to_string{
        {TaskState::UNKNOWN, "UNKNOWN"},
        {TaskState::NEW, "NEW"},
        {TaskState::IN_WORK, "IN_WORK"},
        {TaskState::CANCELLED, "CANCELLED"},
        {TaskState::COMPLETED, "COMPLETED"},
    };

    struct Task : Entity {

        string title;
        string description;
        long long user_id;
        TaskState state;

        Task(
            string_view title = "",
            string_view description = "",
            long long user_id = 0,
            TaskState state = TaskState::UNKNOWN,
            long long id = 0,
            datetime created_at = {},
            datetime updated_at = {},
            optional<datetime> deleted_at = nullopt
        ):
            Entity(id, created_at, updated_at, deleted_at),
            title(title),
            description(description),
            user_id(user_id),
            state(state)
        {}

        Task(const row& task_row):
            Entity(task_row),
            title(task_row[TITLE->name].get<string>().value()),
            description(task_row[DESCRIPTION->name].get<string>().value()),
            user_id(task_row[USER_ID->name].get<long long>().value()),
            state(static_cast<TaskState>(task_row[STATE->name].get<int>().value()))
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "tasks";
            return table;
        }

        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);

            result[TITLE->name] = title;
            result[DESCRIPTION->name] = description;
            result[USER_ID->name] = to_string(user_id);
            result[STATE->name] = to_string(state);

            return result;
        }
    };

}