#include <utils/TaskTrackerApi/TaskTrackerApi.hpp>

namespace Utils::TaskTrackerApi {
    using std::make_unique;
    using std::to_string;
    using nlohmann::json;
    using httplib::Result;
    using httplib::Params;
    using Utils::Api::throw_by_status_or_error;
    using Utils::Api::DELETED_AT_KEY;

    constexpr const char* RESULT_KEY = "result";
    constexpr const char* APPLICATION_JSON_KEY = "application/json";

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

    unique_ptr<vector<Task>> TaskTrackerApi::get_tasks(
        long long user_id,
        optional<TaskState> state,
        optional<datetime> start_at,
        optional<datetime> in_work_at,
        optional<datetime> completed_at,
        optional<datetime> deleted_at
    ) {
        const string path = fmt::format("/user/{}/tasks", user_id);
        Params params;
        if (state.has_value()) {
            params.insert({STATE_KEY, to_string(state.value())});
        }
        if (start_at.has_value()) {
            params.insert({START_AT_KEY, start_at->to_string(DATETIME_FORMAT)});
        }
        if (in_work_at.has_value()) {
            params.insert({IN_WORK_AT_KEY, in_work_at->to_string(DATETIME_FORMAT)});
        }
        if (completed_at.has_value()) {
            params.insert({COMPLETED_AT_KEY, completed_at->to_string(DATETIME_FORMAT)});
        }
        if (deleted_at.has_value()) {
            params.insert({DELETED_AT_KEY, deleted_at->to_string(DATETIME_FORMAT)});
        }
        const Result result = _client.Get(path);
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