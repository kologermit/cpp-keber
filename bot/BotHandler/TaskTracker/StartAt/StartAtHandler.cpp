#include <bot/BotHandler/TaskTracker/StartAt/StartAtHandler.hpp>
#include <bot/BotHandler/TaskTracker/Description/DescriptionHandler.hpp>
#include <bot/BotHandler/TaskTracker/TaskTrackerHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <fmt/core.h>

namespace Bot::BotHandler::TaskTracker::StartAt {
    using std::shared_ptr;
    using std::unique_ptr;
    using std::make_shared;
    using std::make_unique;
    using std::exception;
    using jed_utils::datetime;
    using Utils::TaskTrackerApi::Task;
    using Utils::Datetime::DATE_FORMAT;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButtons;
    using Utils::TGBotApi::Types::ReplyLane;
    using Utils::TGBotApi::Types::ReplyButton;
    using Bot::BotHandler::TaskTracker::Description::DescriptionHandler;
    using Bot::Entity::User::UserScreen;

    const string& StartAtHandler::get_name() const noexcept {
        static const string name = "StartAtHandler";
        return name;
    };

    bool StartAtHandler::check(ptrContext ctx) {
        return 
            ctx->user->screen == UserScreen::ADD_TASK_START_AT
            && ctx->global_ctx->task_tracker_cache->contains(ctx->user->id);
    }

    ptrMessage StartAtHandler::handle(ptrContext ctx) {
        if (ctx->message->text == BACK_WORD) {
            return DescriptionHandler::to_task_description(ctx, false);
        }

        datetime start_at;
        if (ctx->message->text == TOMORROW_WORD) {
            start_at.add_days(1);
        } else if (ctx->message->text != TODAY_WORD) {
            try {
                start_at = datetime::parse(DATE_FORMAT, ctx->message->text);
            } catch (const exception&) {
                return ctx->bot->send_message({
                    .chat_id = ctx->chat->id,
                    .text = fmt::format("{}Не получилось определить дату", ERROR_SYMBOL),
                    .reply_message_id = ctx->message->id,
                });
            }
        }
        ctx->global_ctx->task_tracker_cache->operator[](ctx->user->id).start_at = start_at;
        const unique_ptr<Task> new_task = ctx->global_ctx->api->task_tracker->post_task(
            ctx->user->id,
            ctx->global_ctx->task_tracker_cache->operator[](ctx->user->id).title,
            ctx->global_ctx->task_tracker_cache->operator[](ctx->user->id).description,
            ctx->global_ctx->task_tracker_cache->operator[](ctx->user->id).start_at
        );
        ctx->global_ctx->task_tracker_cache->erase(ctx->user->id);
        return TaskTrackerHandler::to_task_tracker(ctx, fmt::format(
            "<b>{}Создана задача:</b>\n<b>ID:</b> <i>{}</i>\n<b>Название:</b> <i>{}</i>\n<b>Описание:</b> <i>{}</i>\n<b>Начало:</b> <i>{}</i>",
            SUCCESS_SYMBOL,
            new_task->id,
            new_task->title,
            new_task->description,
            new_task->start_at.to_string(DATE_FORMAT)
        ));
    }

    ptrMessage StartAtHandler::to_task_start_at(ptrContext ctx, bool save_start_at) {
        ctx->user->screen = UserScreen::ADD_TASK_START_AT;
        ctx->db->user->update(*ctx->user);
        if (!save_start_at) {
            return nullptr;
        }
        ctx->global_ctx->task_tracker_cache->operator[](ctx->user->id).description = ctx->message->text;
        datetime next_2_days, next_3_days, next_4_days, next_5_days;
        next_2_days.add_days(2);
        next_3_days.add_days(3);
        next_4_days.add_days(4);
        next_5_days.add_days(5);

        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = fmt::format("<b>Сохранил описание:</b> <i>{}</i>", ctx->message->text),
            .reply_message_id = ctx->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{
                ReplyLane{make_shared<ReplyButton>(TODAY_WORD), make_shared<ReplyButton>(TOMORROW_WORD)},
                ReplyLane{make_shared<ReplyButton>(next_2_days.to_string(DATE_FORMAT)), 
                    make_shared<ReplyButton>(next_3_days.to_string(DATE_FORMAT))},
                ReplyLane{make_shared<ReplyButton>(next_4_days.to_string(DATE_FORMAT)), 
                    make_shared<ReplyButton>(next_5_days.to_string(DATE_FORMAT))},
                ReplyLane{make_shared<ReplyButton>(BACK_WORD)},
            })
        });
    }
}