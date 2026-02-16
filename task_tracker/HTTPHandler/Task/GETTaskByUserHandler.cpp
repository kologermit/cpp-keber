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
    using TaskTracker::Entity::Task::GetByUserParams;
    using TaskTracker::Entity::Task::STATE_KEY;
    using TaskTracker::Entity::Task::USER_ID_COLUMN;

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

    const HandlerSignature& GETTaskByUserHandler::get_signature() const noexcept {
        static const HandlerSignature signature = {
            .name = "GETTaskByUserHandler",
            .pattern = "/user/:user_id/tasks",
            .method = RequestHandlerMethod::GET,
            .is_auth = true,
            .path_params = {
                Param{USER_ID_COLUMN, ParamType::INT, true, 0},
            },
            .query_params = {
                Param{STATE_KEY, ParamType::INT, false, TaskState::NEW, TaskState::DELETED},
                Param{CREATED_AT_GTE_PARAM, ParamType::DATETIME, false},
                Param{CREATED_AT_LTE_PARAM, ParamType::DATETIME, false},
                Param{START_AT_GTE_PARAM, ParamType::DATETIME, false},
                Param{START_AT_LTE_PARAM, ParamType::DATETIME, false},
                Param{IN_WORK_AT_GTE_PARAM, ParamType::DATETIME, false},
                Param{IN_WORK_AT_LTE_PARAM, ParamType::DATETIME, false},
                Param{COMPLETED_AT_GTE_PARAM, ParamType::DATETIME, false},
                Param{COMPLETED_AT_LTE_PARAM, ParamType::DATETIME, false},
                Param{DELETED_AT_GTE_PARAM, ParamType::DATETIME, false},
                Param{DELETED_AT_LTE_PARAM, ParamType::DATETIME, false},
            }
        };
        return signature;
    }

    json GETTaskByUserHandler::handle(ptrContext ctx) {

        GetByUserParams params{
            .user_id = ctx->ll_params[USER_ID_COLUMN]
        };
        
        for (auto& [name, value] : map<string, optional<datetime>*>{
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
            if (ctx->datetime_params.contains(name)) {
                *value = ctx->datetime_params[name];
            }
        }

        if (ctx->ll_params.contains(STATE_KEY)) {
            params.state = static_cast<TaskState>(ctx->ll_params[STATE_KEY]);
        }

        unique_ptr<vector<Task>> tasks = ctx->db->task->get_by_user_id(params);
        
        json return_result = json::array();
        for (const Task& task : *tasks) {
            return_result.push_back(task.to_json());
        }
        return return_result;
    }
}
