#include <bot/BotHandler/PatchTask/PatchTaskHandler.hpp>
#include <bot/BotHandler/TaskTracker/TaskTrackerHandler.hpp>
#include <utils/String/String.hpp>
#include <utils/TaskTrackerApi/Task.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <utils/Datetime.hpp>
#include <fmt/format.h>
#include <string_view>
#include <optional>
#include <memory>
#include <vector>
#include <exception>

namespace Bot::BotHandler::PatchTask {

    using Bot::BotHandler::TaskTracker::TaskTrackerHandler;
    using Utils::String::split;
    using Utils::String::join;
    using Utils::TGBotApi::Bot::SendMessageParameters;
    using Utils::TaskTrackerApi::Task;
    using Utils::TaskTrackerApi::TaskState;
    using Utils::Datetime::datetime;
    using Utils::Datetime::DATE_FORMAT;
    using std::unique_ptr;
    using std::string_view;
    using std::optional;
    using std::nullopt;
    using std::vector;
    using std::stoll;
    using std::exception;

    const string& PatchTaskHandler::get_name() const noexcept {
        static const string name = "PatchTaskrHandler";
        return name;
    }

    bool PatchTaskHandler::check(ptrContext ctx) {
        // Пример 
        //   /patch_task task_id description/title/start_at/state
        //   value
        //   ...
        return ctx->message->text.starts_with("/patch_task");
    }

    ptrMessage PatchTaskHandler::handle(ptrContext ctx) {
        const vector<string> lines = split(ctx->message->text, "\n");
        const vector<string> first_line_words = split(lines[0], " ");
        SendMessageParameters message_params = {
            .chat_id = ctx->chat->id,
            .reply_message_id = ctx->message->id,
        };
        if (first_line_words.size() < 3 && lines.size() < 2) {
            return ctx->bot->send_message({
                .chat_id = ctx->chat->id,
                .text = "Неправльное количество параметров!",
                .reply_message_id = ctx->message->id,
            });
        }

        long long task_id;
        try {
            task_id = stoll(first_line_words[1]);
        } catch (const exception& exc) {
            message_params.text = fmt::format(
                "Не получилось определить id задачи: {}. Ошибка {}",
                first_line_words[1],
                exc.what()
            );
            return ctx->bot->send_message(message_params);
        }
        optional<string_view> title;
        optional<string_view> description;
        optional<datetime> start_at;
        optional<TaskState> state;

        if (first_line_words[2] == "title") {
            title = lines[1];
        } else if (first_line_words[2] == "description") {
            description = join(lines.begin() + 1, lines.end(), "\n");
        } else if (first_line_words[2] == "start_at") {
            try {
                start_at = datetime::parse(DATE_FORMAT, lines[1]);
            } catch (const exception& exc) {
                message_params.text = fmt::format(
                    "Не получилось определить дату {}. Ошибка {}",
                    lines[1],
                    exc.what()
                );
                return ctx->bot->send_message(message_params);
            }
        } else if (first_line_words[2] == "state") {
            state = Task::string_to_state(lines[1]);
            if (state.value() == TaskState::UNKNOWN) {
                message_params.text = fmt::format("Не получилось определить состоятие задачи {}", lines[1]);
                return ctx->bot->send_message(message_params);
            }
        } else {
            message_params.text = "Не получилось определить изменяемое поле задачи";
            return ctx->bot->send_message(message_params);
        }

        try {
            unique_ptr<Task> patched_task = ctx->global_ctx->api->task_tracker->patch_task(
                task_id,
                ctx->user->id,
                title,
                description,
                start_at,
                state
            );
            return TaskTrackerHandler::send_task(ctx, *patched_task);
        } catch (const exception& exc) {
            message_params.text = fmt::format("Ошибка при обновлении задачи {}", exc.what());
            return ctx->bot->send_message(message_params);
        }
    }
}