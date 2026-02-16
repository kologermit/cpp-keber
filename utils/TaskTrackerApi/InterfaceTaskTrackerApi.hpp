#pragma once

#include <utils/TaskTrackerApi/Task.hpp>
#include <vector>
#include <memory>

namespace Utils::TaskTrackerApi {
    using std::optional;
    using std::nullopt;
    using std::string_view;
    using std::vector;
    using std::unique_ptr;
    using jed_utils::datetime;
    
    const string& task_state_to_symbol(const TaskState state);

    struct GetTasksParams{
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

    struct InterfaceTaskTrackerApi {
        virtual ~InterfaceTaskTrackerApi() = default;

        virtual unique_ptr<Task> get_task(
            long long id,
            long long user_id
        ) = 0;
        virtual unique_ptr<vector<Task>> get_tasks(const GetTasksParams& params) = 0;
        virtual unique_ptr<Task> post_task(
            long long user_id,
            string_view title,
            optional<string_view> description = nullopt,
            optional<datetime> start_at = nullopt
        ) = 0;
        virtual unique_ptr<Task> patch_task(
            long long id,
            long long user_id,
            optional<string_view> title = nullopt,
            optional<string_view> description = nullopt,
            optional<datetime> start_at = nullopt,
            optional<TaskState> state = nullopt
        ) = 0;
    };
}