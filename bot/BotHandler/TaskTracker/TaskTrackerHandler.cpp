#include <bot/BotHandler/TaskTracker/TaskTrackerHandler.hpp>
#include <bot/BotHandler/Menu/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/BotHandler/TaskTracker/Description/DescriptionHandler.hpp>
#include <utils/TGBotApi/Types.hpp>

namespace Bot::BotHandler::TaskTracker {
    using std::make_unique;
    using std::make_shared;
    using std::unique_ptr;
    using std::set;
    using std::vector;
    using std::min;
    using std::max;
    using std::to_string;
    using std::optional;
    using nlohmann::json;
    using jed_utils::datetime;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::ReplyButtons;
    using Utils::TGBotApi::Types::ReplyLane;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::InlineButton;
    using Utils::TGBotApi::Types::InlineButtons;
    using Utils::TGBotApi::Types::InlineLane;
    using Utils::TGBotApi::Types::InlineKeyboard;
    using Utils::TaskTrackerApi::task_state_to_symbol;
    using Utils::TaskTrackerApi::TaskState;
    using Utils::TaskTrackerApi::Task;
    using Utils::Datetime::DATE_FORMAT;
    using Bot::Entity::User::UserScreen;
    using Bot::BotHandler::Menu::MenuHandler;
    using Bot::BotHandler::TaskTracker::Description::DescriptionHandler;

    const string& TaskTrackerHandler::get_name() const noexcept {
        static const string name = "TaskTracker";
        return name;
    }

    bool TaskTrackerHandler::check(ptrContext ctx) {
        return 
            ctx->user->screen == UserScreen::TASK_TRACKER
            && (
                ctx->access.task_tracker
                || ctx->access.full
            );
    }

    ptrMessage TaskTrackerHandler::to_task_tracker(ptrContext ctx, string_view message_text) {
        ctx->user->screen = UserScreen::TASK_TRACKER;
        ctx->db->user->update(*ctx->user);
        
        string text(message_text);
        if (message_text.empty()) {
            unique_ptr<vector<Task>> new_tasks = ctx->global_ctx->api->task_tracker->get_tasks({
                .user_id = ctx->user->id,
                .state = TaskState::NEW,
            });
            unique_ptr<vector<Task>> in_work_tasks = ctx->global_ctx->api->task_tracker->get_tasks({
                .user_id = ctx->user->id,
                .state = TaskState::IN_WORK,
            });
            const size_t count_tasks = new_tasks->size() + in_work_tasks->size();
            text = (
                count_tasks > 0
                ? fmt::format(
                    COUNT_TASKS_PHRASE,
                    count_tasks
                )
                : ALL_TASLS_COMPLETED_PHRASE
            );
        }

        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = text,
            .reply_message_id = ctx->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{
                ReplyLane{
                    make_shared<ReplyButton>(TODAY_WORD),
                    make_shared<ReplyButton>(TOMORROW_WORD),
                },
                ReplyLane{
                    make_shared<ReplyButton>(NEXT_2_DAYS_WORD),
                    make_shared<ReplyButton>(NEXT_3_DAYS_WORD),
                },
                ReplyLane{
                    make_shared<ReplyButton>(STATISTIC_WORD),
                    make_shared<ReplyButton>(BACK_WORD),
                }
            })
        });
    }
    
    ptrMessage TaskTrackerHandler::handle(ptrContext ctx) {
        if (ctx->message->text == BACK_WORD) {
            return MenuHandler::to_menu(ctx);
        }

        static const set<string> task_buttons{
            TODAY_WORD,
            TOMORROW_WORD,
            NEXT_2_DAYS_WORD,
            NEXT_3_DAYS_WORD,
        };

        if (task_buttons.contains(ctx->message->text)) {
            return TaskTrackerHandler::send_tasks(ctx);
        }

        if (ctx->message->text == STATISTIC_WORD) {
            return TaskTrackerHandler::send_statistic(ctx);
        }

        if (!ctx->global_ctx->task_tracker_cache->contains(ctx->user->id)) {
            ctx->global_ctx->task_tracker_cache->insert({ctx->user->id, {}});
        }

        return DescriptionHandler::to_task_description(ctx, true);
    }

    string get_id_zero_aligment(const vector<Task>& tasks, long long id) {
        static const size_t BLOCK_SIZE = 3;
        long long max_id = -1;
        for (const Task& task : tasks) {
            max_id = max(task.id, max_id);
        }
        size_t aligment_size = to_string(max_id).size() / BLOCK_SIZE * BLOCK_SIZE + BLOCK_SIZE;
        return string(aligment_size - to_string(id).size(), '0');
    }

    string get_task_html(const vector<Task>& tasks, const Task& task) {
        return fmt::format(
            "\n<i>{}{}{}. {}</i>",
            task_state_to_symbol(task.state),
            get_id_zero_aligment(tasks, task.id),
            task.id,
            task.title
        );
    }

    ptrMessage TaskTrackerHandler::send_statistic(ptrContext ctx) {
        const datetime today;
        const datetime today_start(today.get_year(), today.get_month(), today.get_day(), 0, 0, 0);
        const datetime today_end(today.get_year(), today.get_month(), today.get_day(), 23, 59, 59);
        const unique_ptr<vector<Task> > new_tasks = ctx->global_ctx->api->task_tracker->get_tasks({
            .user_id = ctx->user->id,
            .start_at_lte = today_end,
            .state = TaskState::NEW,
        });
        const unique_ptr<vector<Task> > in_work_tasks = ctx->global_ctx->api->task_tracker->get_tasks({
            .user_id = ctx->user->id,
            .start_at_lte = today_end,
            .state = TaskState::IN_WORK,
        });
        const unique_ptr<vector<Task> > completed_tasks = ctx->global_ctx->api->task_tracker->get_tasks({
            .user_id = ctx->user->id,
            .completed_at_gte = today_start,
            .completed_at_lte = today_end,
            .state = TaskState::COMPLETED,
        });
        const unique_ptr<vector<Task> > deleted_tasks = ctx->global_ctx->api->task_tracker->get_tasks({
            .user_id = ctx->user->id,
            .deleted_at_gte = today_start,
            .deleted_at_lte = today_end,
            .state = TaskState::DELETED,
        });
        string message_text = fmt::format("<b>Статистика за сегодня ({})</b>", today.to_string(DATE_FORMAT));
        message_text += fmt::format("\n\n<b>Новые ({}):</b>", new_tasks->size());
        for (const Task& task : *new_tasks) {
            message_text += get_task_html(*new_tasks, task);
        }
        message_text += fmt::format("\n\n<b>В работе ({}):</b>", in_work_tasks->size());
        for (const Task& task : *in_work_tasks) {
            message_text += get_task_html(*in_work_tasks, task);
        }
        message_text += fmt::format("\n\n<b>Завершенные ({}):</b>", completed_tasks->size());
        for (const Task& task : *completed_tasks) {
            message_text += get_task_html(*completed_tasks, task);
        }
        message_text += fmt::format("\n\n<b>Отмененные ({}):</b>", deleted_tasks->size());
        for (const Task& task : *deleted_tasks) {
            message_text += get_task_html(*deleted_tasks, task);
        }
        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = message_text,
            .reply_message_id = ctx->message->id,
        });
    }

    ptrMessage TaskTrackerHandler::send_tasks(ptrContext ctx) {
        datetime start_at{};
        start_at.add_days(
            ctx->message->text == TOMORROW_WORD ? 1
            : ctx->message->text == NEXT_2_DAYS_WORD ? 2
            : ctx->message->text == NEXT_3_DAYS_WORD ? 3
            : 0
        );
        optional<datetime> start_at_gte;
        if (ctx->message->text != TODAY_WORD) {
            start_at_gte = datetime{};
            start_at_gte->add_days(
                ctx->message->text == TOMORROW_WORD ? 1
                : ctx->message->text == NEXT_2_DAYS_WORD ? 2
                : 3
            );  
        }
        const unique_ptr<vector<Task>> new_tasks = ctx->global_ctx->api->task_tracker->get_tasks({
            .user_id = ctx->user->id,
            .start_at_gte = start_at_gte,
            .start_at_lte = start_at,
            .state = TaskState::NEW,
        });
        const unique_ptr<vector<Task>> in_work_tasks = ctx->global_ctx->api->task_tracker->get_tasks({
            .user_id = ctx->user->id,
            .start_at_gte = start_at_gte,
            .start_at_lte = start_at,
            .state = TaskState::IN_WORK,
        });
        vector<Task> tasks(*in_work_tasks);
        tasks.insert(tasks.end(), new_tasks->begin(), new_tasks->end());
        string result_text;
        long long max_id = -1;
        for (const auto& task : tasks) {
            max_id = max(max_id, task.id);
        }
        static const size_t minimal_aligment = 3;
        static const vector<const char*> inline_buttons{
            IN_WORK_SYMBOL,
            COMPLETE_SYMBOL,
            DELETED_SYMBOL,
            INFO_SYMBOL,
            ONE_SYMBOL,
            TWO_SYMBOL,
        };
        const size_t size_of_aligment = to_string(max_id).size() / minimal_aligment * minimal_aligment + minimal_aligment;

        for (size_t i = 0; i < std::min(tasks.size(), size_t(15)); i++) {
            const Task& task = tasks[i];
            InlineLane button_lane;
            for (const auto inline_button : inline_buttons) {
                button_lane.push_back(make_shared<InlineButton>(inline_button, "", json{
                    TASK_TRACKER_CALLBACK_HANDLER_NAME,
                    inline_button,
                    task.id
                }.dump()));
            }
            ctx->bot->send_message({
                .chat_id = ctx->chat->id,
                .text = fmt::format(
                    "<b>{} {}</b>",
                    task_state_to_symbol(task.state),
                    task.title
                ),
                .inline_keyboard = make_unique<InlineKeyboard>(InlineButtons{button_lane})
            });

            string zero_aligment_id = to_string(task.id);
            zero_aligment_id = string(size_of_aligment - zero_aligment_id.size(), '0') + zero_aligment_id;
            
            result_text += fmt::format(
                "<b>{} {}.</b> <i>{}</i>\n",
                task_state_to_symbol(task.state),
                zero_aligment_id,
                task.title
            );
        }
        if (result_text.empty()) {
            result_text = "<b>Задачи не найдены</b>";
        } else {
            result_text = fmt::format("<b>Список задач:</b>\n{}\n<b>Всего задач: {}</b>", result_text, tasks.size());
        }
        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = result_text,
            .reply_message_id = ctx->message->id,
        });
    }
}