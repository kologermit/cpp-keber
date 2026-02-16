#include <task_tracker/HTTPHandler/Task/GETTaskHandler.hpp>

namespace TaskTracker::HTTPHandler::Task {
    using std::unique_ptr;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;
    using Utils::HTTPServer::Handler::Param;
    using Utils::HTTPServer::Handler::ParamType;
    using Utils::Entity::ID_COLUMN;
    using TaskTracker::Entity::Task::Task;
    using TaskTracker::Entity::Task::USER_ID_COLUMN;

    const HandlerSignature& GETTaskHandler::get_signature() const noexcept {
        static const HandlerSignature signature = {
            .name = "GETTaskHandler",
            .pattern = "/user/:user_id/task/:id",
            .method = RequestHandlerMethod::GET,
            .is_auth = true,
            .path_params = {
                Param{ID_COLUMN, ParamType::INT, true, 0},
                Param{USER_ID_COLUMN, ParamType::INT, true, 0},
            },
        };
        return signature;
    }

    json GETTaskHandler::handle(ptrContext ctx) {
        const unique_ptr<Task> task = ctx->db->task->get_by_id(ctx->ll_params[ID_COLUMN], false);
        if (task == nullptr || task->user_id != ctx->ll_params[USER_ID_COLUMN]) {
            return nullptr;
        }
        return task->to_json();
    }
}
