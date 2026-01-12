#include <task_tracker/HTTPHandler/Task/GETTaskByUserHandler.hpp>

namespace TaskTracker::HTTPHandler::Task {
    using std::unique_ptr;
    using std::vector;
    using jed_utils::datetime;
    using Utils::Datetime::DATETIME_FORMAT;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;
    using Utils::HTTPServer::Handler::Param;
    using Utils::HTTPServer::Handler::ParamType;
    using Utils::Entity::DELETED_AT_COLUMN;
    using TaskTracker::Entity::Task::Task;
    using TaskTracker::Entity::Task::TaskState;
    using TaskTracker::Entity::Task::STATE_KEY;
    using TaskTracker::Entity::Task::USER_ID_COLUMN;
    using TaskTracker::Entity::Task::START_AT_COLUMN;
    using TaskTracker::Entity::Task::IN_WORK_AT_COLUMN;
    using TaskTracker::Entity::Task::COMPLETED_AT_COLUMN;

    const HandlerSignature& GETTaskByUserHandler::get_signature() const noexcept {
        static const HandlerSignature signature = {
            .name = "GETTaskByUser",
            .pattern = "/user/:user_id/tasks",
            .method = RequestHandlerMethod::GET,
            .is_auth = true,
            .path_params = {
                Param{USER_ID_COLUMN, ParamType::INT, true, 0},
            },
            .query_params = {
                Param{STATE_KEY, ParamType::INT, false, TaskState::NEW, TaskState::DELETED},
                Param{START_AT_COLUMN, ParamType::DATETIME, false},
                Param{IN_WORK_AT_COLUMN, ParamType::DATETIME, false},
                Param{COMPLETED_AT_COLUMN, ParamType::DATETIME, false},
                Param{DELETED_AT_COLUMN, ParamType::DATETIME, false},
            }
        };
        return signature;
    }

    json GETTaskByUserHandler::handle(ptrContext ctx) {
        optional<datetime> start_at;
        optional<datetime> in_work_at;
        optional<datetime> completed_at;
        optional<datetime> deleted_at;
        optional<TaskState> state;
        
        for (auto& [name, value] : map<string, optional<datetime>*>{
            {START_AT_COLUMN, &start_at},
            {IN_WORK_AT_COLUMN, &in_work_at},
            {COMPLETED_AT_COLUMN, &completed_at},
            {DELETED_AT_COLUMN, &deleted_at},
        }) {
            if (ctx->datetime_params.contains(name)) {
                *value = ctx->datetime_params[name];
            }
        }

        if (ctx->ll_params.contains(STATE_KEY)) {
            state = static_cast<TaskState>(ctx->ll_params[STATE_KEY]);
        }

        unique_ptr<vector<Task>> tasks = ctx->db->task->get_by_user_id(
            ctx->ll_params[USER_ID_COLUMN],
            start_at,
            in_work_at,
            completed_at,
            deleted_at,
            state
        );
        
        json return_result = json::array();
        for (const Task& task : *tasks) {
            return_result.push_back(task.to_json());
        }
        return return_result;
    }
}
