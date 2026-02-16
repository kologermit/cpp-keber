#pragma once

#include <task_tracker/Entity/Task/InterfaceTaskRepository.hpp>

namespace TaskTracker::Entity::Task {
    struct TaskRepository final : InterfaceTaskRepository {
        unique_ptr<vector<Task>> get_by_user_id(const GetByUserParams& params) override;

        explicit TaskRepository(connection& db): InterfaceTaskRepository(db) {};
    };
}