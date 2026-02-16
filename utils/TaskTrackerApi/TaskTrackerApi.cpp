#include <utils/TaskTrackerApi/TaskTrackerApi.hpp>
#include <fmt/core.h>

namespace Utils::TaskTrackerApi {
    using std::make_unique;
    using std::to_string;
    using std::vector;
    using std::pair;
    using nlohmann::json;
    using httplib::Result;
    using httplib::Params;
    using Utils::Api::throw_by_status_or_error;
    using Utils::Api::DELETED_AT_KEY;

    constexpr const char* RESULT_KEY = "result";
    constexpr const char* APPLICATION_JSON_KEY = "application/json";

    const string& task_state_to_symbol(const TaskState state) {
        static const map<TaskState, string> m{
            {TaskState::UNKNOWN, "🚫"},
            {TaskState::NEW, "🆕"},
            {TaskState::IN_WORK, "🛠️"},
            {TaskState::COMPLETED, "✅"},
            {TaskState::DELETED, "❌"}, 
        };
        return m.at(state);
    }

    TaskTrackerApi::TaskTrackerApi(string_view base_url, string_view auth_key):
        InterfaceTaskTrackerApi(),
        Api(base_url, auth_key)
    {}

    unique_ptr<Task> TaskTrackerApi::get_task(
        long long id,
        long long user_id
    ) {
        const string path = fmt::format("/user/{}/task/{}", user_id, id);
        const Result result = _client.Get(path);
        throw_by_status_or_error(_base_url, path, result);
        json body = json::parse(result->body);
        if (body[RESULT_KEY].is_null()) {
            return nullptr;
        }
        return make_unique<Task>(body[RESULT_KEY]);
    }

    constexpr const char* CREATED_AT_GTE_PARAM = "created_at_gte";
    constexpr const char* CREATED_AT_LTE_PARAM = "created_at_lte";
    constexpr const char* START_AT_GTE_PARAM = "start_at_gte";
    constexpr const char* START_AT_LTE_PARAM = "start_at_lte";
    constexpr const char* IN_WORK_AT_GTE_PARAM = "in_work_at_gte";
    constexpr const char* IN_WORK_AT_LTE_PARAM = "in_work_at_lte";
    constexpr const char* COMPLETED_AT_GTE_PARAM = "completed_at_gte";
    constexpr const char* COMPLETED_AT_LTE_PARAM = "completed_at_lte";
    constexpr const char* DELETED_AT_GTE_PARAM = "deleted_at_gte";
    constexpr const char* DELETED_AT_LTE_PARAM = "deleted_at_lte";

    unique_ptr<vector<Task>> TaskTrackerApi::get_tasks(const GetTasksParams& params) {
        const string path = fmt::format("/user/{}/tasks", params.user_id);
        Params query_params;
        if (params.state.has_value()) {
            query_params.insert({STATE_KEY, to_string(params.state.value())});
        }
        for (const auto& [key, value] : vector<pair<const char*, const optional<datetime>*>>{
            {CREATED_AT_GTE_PARAM, &params.created_at_gte},
            {CREATED_AT_LTE_PARAM, &params.created_at_lte},
            {START_AT_GTE_PARAM, &params.start_at_gte},
            {START_AT_LTE_PARAM, &params.start_at_lte},
            {IN_WORK_AT_GTE_PARAM, &params.in_work_at_gte},
            {IN_WORK_AT_LTE_PARAM, &params.in_work_at_lte},
            {COMPLETED_AT_GTE_PARAM, &params.completed_at_gte},
            {COMPLETED_AT_LTE_PARAM, &params.completed_at_lte},
            {DELETED_AT_GTE_PARAM, &params.deleted_at_gte},
            {DELETED_AT_LTE_PARAM, &params.deleted_at_lte},
        }) {
            if (value->has_value()) {
                query_params.insert({key, value->value().to_string(DATETIME_FORMAT)});
            }
        }
        const Result result = _client.Get(path, query_params, {});
        throw_by_status_or_error(_base_url, path, result);
        json body = json::parse(result->body);
        unique_ptr<vector<Task>> return_result = make_unique<vector<Task>>();
        for (const json& json_task : body[RESULT_KEY]) {
            return_result->emplace_back(json_task);
        }
        return return_result;
    }

    unique_ptr<Task> TaskTrackerApi::post_task(
        long long user_id,
        string_view title,
        optional<string_view> description,
        optional<datetime> start_at
    ) {
        static const string path = "/task";
        json request_body{
            {USER_ID_KEY, user_id},
            {TITLE_KEY, title},
        };
        if (description.has_value()) {
            request_body[DESCRIPTION_KEY] = description.value();
        }
        if (start_at.has_value()) {
            request_body[START_AT_KEY] = start_at->to_string(DATETIME_FORMAT);
        }
        const Result result = _client.Post(path, request_body.dump(), APPLICATION_JSON_KEY);
        throw_by_status_or_error(_base_url, path, result);
        return make_unique<Task>(json::parse(result->body)[RESULT_KEY]);
    }

    unique_ptr<Task> TaskTrackerApi::patch_task(
        long long id,
        long long user_id,
        optional<string_view> title,
        optional<string_view> description,
        optional<datetime> start_at,
        optional<TaskState> state
    ) {
        const string path = fmt::format("/user/{}/task/{}", user_id, id);
        json request_body{};
        if (title.has_value()) {
            request_body[TITLE_KEY] = title;
        }
        if (description.has_value()) {
            request_body[DESCRIPTION_KEY] = description;
        }
        if (start_at.has_value()) {
            request_body[START_AT_KEY] = start_at->to_string(DATETIME_FORMAT);
        }
        if (state.has_value()) {
            request_body[STATE_KEY] = state.value();
        }
        const Result result = _client.Patch(path, request_body.dump(), APPLICATION_JSON_KEY);
        throw_by_status_or_error(_base_url, path, result);
        return make_unique<Task>(json::parse(result->body)[RESULT_KEY]);
    }
}