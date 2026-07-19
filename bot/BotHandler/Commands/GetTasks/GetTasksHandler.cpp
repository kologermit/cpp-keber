#include <bot/BotHandler/Commands/GetTasks/GetTasksHandler.hpp>
#include <bot/BotHandler/TaskTracker/TaskTrackerHandler.hpp>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <optional>
#include <fmt/format.h>
#include <utils/Datetime.hpp>
#include <utils/String/String.hpp>
#include <utils/TaskTrackerApi/TaskTrackerApi.hpp>

namespace Bot::BotHandler::Commands::GetTasks {
    using std::count;
    using std::vector;
    using std::unique_ptr;
    using std::map;
    using std::set;
    using std::nullopt;
    using Bot::BotHandler::TaskTracker::TaskTrackerHandler;
    using Utils::Datetime::datetime;
    using Utils::Datetime::DATE_FORMAT;
    using Utils::Datetime::DATETIME_FORMAT;
    using Utils::String::split;
    using Utils::TaskTrackerApi::TaskState;
    using Utils::TaskTrackerApi::Task;
    using Utils::TaskTrackerApi::GetTasksParams;

    const string& GetTasksHandler::get_name() const noexcept {
        static const string name = "GetTasksrHandler";
        return name;
    }

    bool GetTasksHandler::check(ptrContext ctx) {
        // Пример /get_tasks date state1,state2...
        // date - дата в формате YYYY-MM-DD, по умолчанию текущая дата
        // state - состояние задачи, по умолчанию new, возможеные: completed, deleted, in_work, new
        return ctx->message->text.starts_with("/get_tasks");
    }

    ptrMessage GetTasksHandler::handle(ptrContext ctx) {
        vector<string> words = split(ctx->message->text, " ");
        if (words.size() > 3) {
            return ctx->bot->send_message({
                .chat_id = ctx->chat->id,
                .text = "Указано неверное количество параметров",
                .reply_message_id = ctx->message->id,
            });
        }

        datetime date;
        if (words.size() >= 2 && words[1] != "-") {
            try {
                date = datetime::parse(DATE_FORMAT, words[1]);
            } catch (const std::exception& e) {
                return ctx->bot->send_message({
                    .chat_id = ctx->chat->id,
                    .text = fmt::format("Неверный формат даты. Используйте YYYY-MM-DD ({})",
                        date.to_string(DATE_FORMAT)),
                    .reply_message_id = ctx->message->id,
                });
            }
        }
        
        datetime start(
            date.get_year(),
            date.get_month(),
            date.get_day(),
            0, 0, 0
        );
        datetime end = start;
        end.add_days(1);

        set<TaskState> states = {TaskState::NEW};
        const static map<string, TaskState> state_map = {
            {"new", TaskState::NEW},
            {"completed", TaskState::COMPLETED},
            {"deleted", TaskState::DELETED},
            {"in_work", TaskState::IN_WORK}
        };
        if (words.size() == 3) {
            states.clear();
            vector<string> state_strings = split(words[2], ",");
            for (const string& state_str : state_strings) {
                const map<string, TaskState>::const_iterator it = state_map.find(state_str);
                if (it != state_map.end()) {
                    states.insert(it->second);
                } else {
                    return ctx->bot->send_message({
                        .chat_id = ctx->chat->id,
                        .text = "Указано неправильное состояние задачи: " + state_str,
                        .reply_message_id = ctx->message->id,
                    });
                }
            }
        }

        vector<Task> tasks;
        for (const TaskState& state : states) {
            GetTasksParams params{.user_id=ctx->user->id, .state=state};
            if (state == TaskState::COMPLETED) {
                params.completed_at_gte = start;
                params.completed_at_lte = end;
            } else if (state == TaskState::DELETED) {
                params.deleted_at_gte = start;
                params.deleted_at_lte = end;
            } else if (state == TaskState::IN_WORK) {
                params.in_work_at_gte = start;
                params.in_work_at_lte = end;
            } else if (state == TaskState::NEW) {
                params.start_at_gte = start;
                params.start_at_lte = end;
            }
            unique_ptr<vector<Task> > result = ctx->global_ctx->api->task_tracker->get_tasks(params);
            tasks.insert(tasks.end(), result->begin(), result->end());
        }

        return TaskTrackerHandler::send_tasks(ctx, tasks);
    }
}