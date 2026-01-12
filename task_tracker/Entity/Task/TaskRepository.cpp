#include <task_tracker/Entity/Task/TaskRepository.hpp>
#include <utils/Entity/exec_templates.hpp>

namespace TaskTracker::Entity::Task {

    using std::to_string;
    using Utils::Entity::exec_select;

    unique_ptr<vector<Task>> TaskRepository::get_by_user_id(
            long long user_id,
            optional<datetime> start_at,
            optional<datetime> in_work_at,
            optional<datetime> completed_at,
            optional<datetime> deleted_at
        ) {
        
    }
}