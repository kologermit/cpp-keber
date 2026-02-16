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

    struct GetByUserParams {
        long long user_id;
        optional<datetime> created_at_gte = nullopt;
        optional<datetime> created_at_lte = nullopt;
        optional<datetime> start_at_gte = nullopt;
        optional<datetime> start_at_lte = nullopt;
        optional<datetime> in_work_at_gte = nullopt;
        optional<datetime> in_work_at_lte = nullopt;
        optional<datetime> completed_at_gte = nullopt; 
        optional<datetime> completed_at_lte = nullopt;
        optional<datetime> deleted_at_gte = nullopt;
        optional<datetime> deleted_at_lte = nullopt;
        optional<TaskState> state = nullopt;
    };

    struct InterfaceTaskRepository : Repository<Task> {
        virtual unique_ptr<vector<Task>> get_by_user_id(const GetByUserParams& params) = 0;

        explicit InterfaceTaskRepository(connection& db): Repository(db) {}
    };
}