#include <task_tracker/Entity/Task/TaskRepository.hpp>
#include <utils/Entity/exec_templates.hpp>

namespace TaskTracker::Entity::Task {

    using std::to_string;
    using Utils::Entity::create_rows_in_enum_table_if_empty;
    using Utils::Entity::exec_select_many;

    TaskRepository::TaskRepository(connection& db):
    InterfaceTaskRepository(db) {
        create_rows_in_enum_table_if_empty(_db, TASK_STATES_TABLE, map_task_state_to_string);
    }

    unique_ptr<vector<Task>> TaskRepository::get_by_user_id(long long user_id, bool check_deleted) {
        return exec_select_many<Task>(
            _db,
            Task::get_table_name(),
            {{USER_ID->name, to_string(user_id)}},
            check_deleted
        );
    }
}