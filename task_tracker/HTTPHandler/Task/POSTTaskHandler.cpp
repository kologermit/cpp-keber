#include <task_tracker/HTTPHandler/Task/POSTTaskHandler.hpp>

namespace TaskTracker::HTTPHandler::Task {

    using std::unique_ptr;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;
    using Utils::HTTPServer::Handler::Param;
    using Utils::HTTPServer::Handler::ParamType;
    using Utils::BotApi::User;
    using Utils::Datetime::DATETIME_FORMAT;
    using jed_utils::datetime;
    using TaskTracker::Entity::Task::TaskState;
    using TaskTracker::Entity::Task::Task;
    using TaskTracker::Entity::Task::TITLE_COLUMN;
    using TaskTracker::Entity::Task::DESCRIPTION_COLUMN;
    using TaskTracker::Entity::Task::USER_ID_COLUMN;
    using TaskTracker::Entity::Task::START_AT_COLUMN;

    const HandlerSignature& POSTTaskHandler::get_signature() const noexcept {
        static const HandlerSignature signature = {
            .name = "POSTTask",
            .pattern = "/task",
            .method = RequestHandlerMethod::POST,
            .is_auth = true,
            .is_json_body = true,
            .body_params = {
                Param{TITLE_COLUMN, ParamType::STRING},
                Param{DESCRIPTION_COLUMN, ParamType::STRING, false},
                Param{USER_ID_COLUMN, ParamType::INT, true, 0},
                Param{START_AT_COLUMN, ParamType::DATETIME, false},
            }
        };
        return signature;
    }

    json POSTTaskHandler::handle(ptrContext ctx) {
        const unique_ptr<User> user = ctx->global_ctx->bot_api->get_user(
            ctx->json_body->at(USER_ID_COLUMN).get<long long>());
        if (user == nullptr) {
            ctx->response.status = 400;
            return "user not found";
        }
        const unique_ptr<Task> task = ctx->db->task->create(Task(
            ctx->json_body->at(TITLE_COLUMN).get<string>(),
            ctx->json_body->value<string>(DESCRIPTION_COLUMN, ""),
            user->id,
            (
                ctx->json_body->contains(START_AT_COLUMN)
                ? datetime::parse(DATETIME_FORMAT, ctx->json_body->at(START_AT_COLUMN).get<string>())
                : datetime{}
            )
        ));
        return task->to_json();
    }
}
