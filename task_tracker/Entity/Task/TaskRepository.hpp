#pragma once

#include <task_tracker/Entity/Task/InterfaceTaskRepository.hpp>

namespace TaskTracker::Entity::Task {
    struct TaskRepository final : InterfaceTaskRepository {
        unique_ptr<vector<Task>> get_by_user_id(long long user_id, bool check_deleted) override;

        explicit TaskRepository(connection& db);
    };
}