#include <bot/BotHandler/GetTask/GetTaskHandler.hpp>
#include <bot/BotHandler/TaskTracker/TaskTrackerHandler.hpp>
#include <pystring.h>
#include <fmt/format.h>

namespace Bot::BotHandler::GetTask {

    using Bot::BotHandler::TaskTracker::TaskTrackerHandler;
    using Utils::TaskTrackerApi::Task;
    using pystring::split;
    using std::exception;
    using std::vector;
    using std::stoll;
    using std::unique_ptr;

    const string& GetTaskHandler::get_name() const noexcept {
        static const string name = "GetTaskHandler";
        return name;
    }

    bool GetTaskHandler::check(ptrContext ctx) {
        // Пример /get_task task_id
        return ctx->message->text.starts_with("/get_task ");
    }

    ptrMessage GetTaskHandler::handle(ptrContext ctx) {
        vector<string> words = split(ctx->message->text, " ");
        long long task_id;
        try {
            task_id = stoll(words[1]);
        } catch (const exception& exc) {
            return ctx->bot->send_message({
                .chat_id = ctx->chat->id,
                .text = fmt::format("Не получилось определить id {}. Ошибка {}", words[1], exc.what()),
                .reply_message_id = ctx->message->id,
            });
        }

        unique_ptr<Task> task = ctx->global_ctx->api->task_tracker->get_task(task_id, ctx->user->id);
        if (task == nullptr) {
            return ctx->bot->send_message({
                .chat_id = ctx->chat->id,
                .text = "Задача не найдена",
                .reply_message_id = ctx->message->id,
            });
        }

        return TaskTrackerHandler::send_task(ctx, *task);
    }
}