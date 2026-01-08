#pragma once

#include <task_tracker/Entity/Task/Task.hpp>
#include <utils/Entity/Repository.hpp>
#include <vector>

namespace TaskTracker::Entity::Task {

    using std::unique_ptr;
    using std::vector;
    using Utils::Entity::Repository;
    using pqxx::connection;

    struct InterfaceTaskRepository : Repository<Task> {
        virtual unique_ptr<vector<Task>> get_by_user_id(long long user_id, bool check_deleted = false) = 0;

        explicit InterfaceTaskRepository(connection& db): Repository(db) {}
    };
}