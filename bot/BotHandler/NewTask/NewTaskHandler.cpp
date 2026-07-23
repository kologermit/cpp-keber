#include <bot/BotHandler/NewTask/NewTaskHandler.hpp>
#include <bot/BotHandler/TaskTracker/StartAt/StartAtHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <utils/Datetime.hpp>
#include <utils/TaskTrackerApi/Task.hpp>
#include <fmt/format.h>
#include <pystring.h>
#include <memory>
#include <string>
#include <vector>
#include <exception>

namespace Bot::BotHandler::NewTask {

    using Bot::BotHandler::EMPTY_WORD;
    using Bot::BotHandler::TaskTracker::StartAt::StartAtHandler;
    using Utils::Datetime::datetime;
    using Utils::Datetime::DATE_FORMAT;
    using pystring::split;
    using pystring::join;
    using pystring::strip;
    using Utils::TaskTrackerApi::Task;
    using std::unique_ptr;
    using std::exception;
    using std::vector;
    using std::string;

    const string& NewTaskHandler::get_name() const noexcept {
        static const string name = "NewTaskrHandler";
        return name;
    }

    bool NewTaskHandler::check(ptrContext ctx) {
        // Пример 
        //   /new_task date
        //   title
        //   description
        //   description
        //   ...
        // 
        // date - необязательный парамерт, по умолчанию сегодняшняя дата
        // title - обязательный параметр, может занимать всю строку
        // description - необязательный параметр, занимает все строки после названия 
        return ctx->message->text.starts_with("/new_task");
    }

    ptrMessage NewTaskHandler::handle(ptrContext ctx) {
        const vector<string> lines = split(ctx->message->text, "\n");
        if (lines.size() < 2) {
            return ctx->bot->send_message({
                .chat_id = ctx->chat->id,
                .text = "Нет названия задачи!",
                .reply_message_id = ctx->message->id,
            });
        }
        const string title = strip(lines[1]);

        datetime start_at;
        const vector<string> words_first_line = split(lines[0], " ");
        if (words_first_line.size() >= 2) {
            const string expected_date = words_first_line[1];
            try {
                start_at = datetime::parse(DATE_FORMAT, expected_date);
            } catch (const exception& exc) {
                return ctx->bot->send_message({
                    .chat_id = ctx->chat->id,
                    .text = fmt::format("Неполучилось определить дату {}. Ошибка {}",
                        expected_date,
                        exc.what()
                    ),
                    .reply_message_id = ctx->message->id,
                });
            }
        }

        string description = strip(join("\n", vector<string>(lines.begin() + 2, lines.end())));
        if (description.empty()) {
            description = EMPTY_WORD;
        }

        unique_ptr<Task> task = ctx->global_ctx->api->task_tracker->post_task(
            ctx->user->id,
            title,
            description,
            start_at
        );

        return StartAtHandler::send_new_task(ctx, *task);
    }
}