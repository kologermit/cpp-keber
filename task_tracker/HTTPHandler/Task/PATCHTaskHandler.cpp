#include <task_tracker/HTTPHandler/Task/PATCHTaskHandler.hpp>

namespace TaskTracker::HTTPHandler::Task {
    using std::unique_ptr;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;
    using Utils::HTTPServer::Handler::Param;
    using Utils::HTTPServer::Handler::ParamType;
    using Utils::BotApi::User;
    using Utils::Datetime::DATETIME_FORMAT;
    using Utils::Entity::ID_COLUMN;
    using jed_utils::datetime;
    using TaskTracker::Entity::Task::TaskState;
    using TaskTracker::Entity::Task::Task;
    using TaskTracker::Entity::Task::TITLE_COLUMN;
    using TaskTracker::Entity::Task::DESCRIPTION_COLUMN;
    using TaskTracker::Entity::Task::USER_ID_COLUMN;
    using TaskTracker::Entity::Task::START_AT_COLUMN;
    using TaskTracker::Entity::Task::STATE_KEY;

    const HandlerSignature& PATCHTaskHandler::get_signature() const noexcept {
        static const HandlerSignature signature = {
            .name = "PATCHTask",
            .pattern = "/user/:user_id/task/:id",
            .method = RequestHandlerMethod::PATCH,
            .is_auth = true,
            .is_json_body = true,
            .path_params = {
                Param{ID_COLUMN, ParamType::INT, true, 0},
                Param{USER_ID_COLUMN, ParamType::INT, true, 0},
            },
            .body_params = {
                Param{TITLE_COLUMN, ParamType::STRING, false, 1, 1000},
                Param{DESCRIPTION_COLUMN, ParamType::STRING, false, 1, 1000},
                Param{START_AT_COLUMN, ParamType::DATETIME, false},
                Param{STATE_KEY, ParamType::INT, false, TaskState::NEW, TaskState::DELETED},
            }
        };
        return signature;
    }

    json PATCHTaskHandler::handle(ptrContext ctx) {
        const unique_ptr<Task> task = ctx->db->task->get_by_id(ctx->ll_params.at(ID_COLUMN));
        if (task == nullptr || task->user_id != ctx->ll_params.at(USER_ID_COLUMN)) {
            ctx->response.status = 400;
            return "task not found";
        }

        if (ctx->json_body->contains(TITLE_COLUMN)) {
            task->title = ctx->json_body->at(TITLE_COLUMN).get<string>();
        }

        if (ctx->json_body->contains(DESCRIPTION_COLUMN)) {
            task->description = ctx->json_body->at(DESCRIPTION_COLUMN).get<string>();
        }

        if (ctx->json_body->contains(START_AT_COLUMN)) {
           task->start_at = datetime::parse(DATETIME_FORMAT, ctx->json_body->at(START_AT_COLUMN).get<string>());
        }

        if (ctx->json_body->contains(STATE_KEY)) {
            switch (ctx->json_body->at(STATE_KEY).get<int>()) {
                case TaskState::NEW:
                    task->in_work_at = nullopt;
                    task->in_work_at = nullopt;
                    task->completed_at = nullopt;
                    task->deleted_at = nullopt;
                    break;
                case TaskState::IN_WORK:
                    task->in_work_at = datetime{};
                    break;
                case TaskState::COMPLETED:
                    task->completed_at = datetime{};
                    break;
                case TaskState::DELETED:
                    ctx->db->task->del(task->id);
                    break;
                default:;
            }
        }

        const unique_ptr<Task> new_task = ctx->db->task->update(*task);

        return new_task->to_json();
    }
}
