#include <task_tracker/HTTPHandler/Task/GETTaskByUserHandler.hpp>

namespace TaskTracker::HTTPHandler::Task {
    using std::unique_ptr;
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
        ctx->db->task->get_by_user_id()
    }
}
