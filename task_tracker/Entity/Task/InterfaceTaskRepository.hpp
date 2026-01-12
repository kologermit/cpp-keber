#pragma once

#include <task_tracker/Entity/Task/Task.hpp>
#include <utils/Entity/Repository.hpp>
#include <vector>
#include <datetime/datetime.h>

namespace TaskTracker::Entity::Task {

    using std::unique_ptr;
    using std::optional;
    using std::nullopt;
    using std::vector;
    using jed_utils::datetime;
    using Utils::Entity::Repository;
    using pqxx::connection;

    struct InterfaceTaskRepository : Repository<Task> {
        virtual unique_ptr<vector<Task>> get_by_user_id(
            long long user_id,
            optional<datetime> start_at = nullopt,
            optional<datetime> in_work_at = nullopt,
            optional<datetime> completed_at = nullopt,
            optional<datetime> deleted_at = nullopt,
            optional<TaskState> state = nullopt
        ) = 0;

        explicit InterfaceTaskRepository(connection& db): Repository(db) {}
    };
}