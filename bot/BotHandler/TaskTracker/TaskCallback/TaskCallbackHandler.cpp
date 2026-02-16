#include <bot/BotHandler/TaskTracker/TaskCallback/TaskCallbackHandler.hpp>
#include <bot/BotHandler/TaskTracker/TaskTrackerHandler.hpp>
#include <bot/BotHandler/Keys.hpp>

namespace Bot::BotHandler::TaskTracker::TaskCallback {
    using std::unique_ptr;
    using std::nullopt;
    using nlohmann::json;
    using Utils::Datetime::DATE_FORMAT;
    using Utils::TaskTrackerApi::TaskState;
    using Utils::TaskTrackerApi::task_state_to_symbol;
    using Utils::TaskTrackerApi::Task;

    const string& TaskCallbackHandler::get_name() const noexcept {
        static const string name = "TaskCallbackHandler";
        return name;
    }
    bool TaskCallbackHandler::check(ptrContext ctx) {
        if (ctx->callback == nullptr || !json::accept(ctx->callback->data)) {
            return false;
        }
        const json data = json::parse(ctx->callback->data);
        return
            data.is_array()
            && data.size() == 3
            && data[0].is_string()
            && data[1].is_string()
            && data[2].is_number_integer()
            && data[0].get<string>() == TASK_TRACKER_CALLBACK_HANDLER_NAME;
    }
    ptrMessage TaskCallbackHandler::handle(ptrContext ctx) {
        ctx->bot->answer_callback_query(ctx->callback->id);
        const json data = json::parse(ctx->callback->data);
        string button = data[1].get<string>();
        long long task_id = data[2].get<long long>();
        unique_ptr<Task> task = ctx->global_ctx->api->task_tracker->get_task(task_id, ctx->callback->user_id);
        if (task == nullptr) {
            return ctx->bot->send_message({
                .chat_id = ctx->chat->id,
                .text = fmt::format("<b>{} Задача {} не найдена!</b>", ERROR_SYMBOL, task_id),
                .reply_message_id = ctx->message->id,
            });
        }

        unique_ptr<Task> new_task;

        if (button == COMPLETE_SYMBOL || button == DELETED_SYMBOL || button == IN_WORK_SYMBOL) {
            new_task = ctx->global_ctx->api->task_tracker->patch_task(
                task->id,
                task->user_id,
                nullopt,
                nullopt,
                nullopt,
                (
                    button == COMPLETE_SYMBOL ? TaskState::COMPLETED 
                    : button == DELETED_SYMBOL ? TaskState::DELETED
                    : TaskState::IN_WORK
                )
            );
        } else if (button == ONE_SYMBOL || button == TWO_SYMBOL) {
            datetime start_at;
            start_at.add_days(button == ONE_SYMBOL ? 1 : 2);
            new_task = ctx->global_ctx->api->task_tracker->patch_task(
                task->id,
                task->user_id,
                nullopt,
                nullopt,
                start_at
            );
        } else {
            new_task = std::move(task);
        }

        return ctx->bot->edit_text(
            ctx->chat->id,
            ctx->message->id,
            fmt::format(
                "<b>{}{}. {}\n\n{}\n\n{}</b>",
                task_state_to_symbol(new_task->state),
                new_task->id,
                new_task->title,
                new_task->description,
                new_task->start_at.to_string(DATE_FORMAT)
            )
        );
    }
}