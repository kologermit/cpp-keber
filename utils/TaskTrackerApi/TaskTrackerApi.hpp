#pragma once

#include <utils/TaskTrackerApi/InterfaceTaskTrackerApi.hpp>
#include <utils/Api/Api.hpp>

namespace Utils::TaskTrackerApi {
    using std::string_view;
    using Utils::Api::Api;

    struct TaskTrackerApi final : InterfaceTaskTrackerApi, Api {
        TaskTrackerApi(string_view base_url, string_view auth_key);

        unique_ptr<Task> get_task(
            long long id,
            long long user_id
        ) override;
        unique_ptr<vector<Task>> get_tasks(
            long long user_id,
            optional<TaskState> state = nullopt,
            optional<datetime> start_at = nullopt,
            optional<datetime> in_work_at = nullopt,
            optional<datetime> completed_at = nullopt,
            optional<datetime> deleted_at = nullopt
        ) override;
        unique_ptr<Task> post_task(
            long long user_id,
            string_view title,
            optional<string_view> description = nullopt,
            optional<datetime> start_at = nullopt
        ) override;
        unique_ptr<Task> patch_task(
            long long id,
            long long user_id,
            optional<string_view> title = nullopt,
            optional<string_view> description = nullopt,
            optional<datetime> start_at = nullopt,
            optional<TaskState> state = nullopt
        ) override;
    };
}