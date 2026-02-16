#include <bot/BotHandler/TaskTracker/Description/DescriptionHandler.hpp>
#include <bot/BotHandler/TaskTracker/TaskTrackerHandler.hpp>
#include <bot/BotHandler/TaskTracker/StartAt/StartAtHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <fmt/core.h>

namespace Bot::BotHandler::TaskTracker::Description {
    using std::shared_ptr;
    using std::make_shared;
    using std::make_unique;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButtons;
    using Utils::TGBotApi::Types::ReplyLane;
    using Utils::TGBotApi::Types::ReplyButton;
    using Bot::BotHandler::TaskTracker::StartAt::StartAtHandler;
    using Bot::Entity::User::UserScreen;

    const string& DescriptionHandler::get_name() const noexcept {
        static const string name = "DescriptionHandler";
        return name;
    }

    bool DescriptionHandler::check(ptrContext ctx) {
        return 
            ctx->user->screen == UserScreen::ADD_TASK_DESCRIPTION
            && ctx->global_ctx->task_tracker_cache->contains(ctx->user->id);
    }

    ptrMessage DescriptionHandler::handle(ptrContext ctx) {
        if (ctx->message->text == BACK_WORD) {
            return TaskTrackerHandler::to_task_tracker(ctx);
        }
        return StartAtHandler::to_task_start_at(ctx, true);
    }

    ptrMessage DescriptionHandler::to_task_description(ptrContext ctx, bool save_title) {
        ctx->user->screen = UserScreen::ADD_TASK_DESCRIPTION;
        ctx->db->user->update(*ctx->user);
        if (!save_title) {
            return nullptr;
        }
        ctx->global_ctx->task_tracker_cache->operator[](ctx->user->id).title = ctx->message->text;
        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = fmt::format("Записал название: <i>{}</i>\n\n<b>Введи описание:</b>", ctx->message->text),
            .reply_message_id = ctx->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(ReplyKeyboard(ReplyButtons{
                ReplyLane{make_shared<ReplyButton>(EMPTY_WORD)},
                ReplyLane{make_shared<ReplyButton>(BACK_WORD)},
            }))
        });
    }
}