#pragma once

#include <task_tracker/Entity/Task/InterfaceTaskRepository.hpp>

namespace TaskTracker::Entity::Task {
    struct TaskRepository final : InterfaceTaskRepository {
        unique_ptr<vector<Task>> get_by_user_id(
            long long user_id,
            optional<datetime> start_at,
            optional<datetime> in_work_at,
            optional<datetime> completed_at,
            optional<datetime> deleted_at
        ) override;

        explicit TaskRepository(connection& db): InterfaceTaskRepository(db) {};
    };
}