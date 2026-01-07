#include <bot/BotHandler/NotFound/NotFoundHandler.hpp>
#include <bot/BotHandler/Keys.hpp>

namespace Bot::BotHandler::NotFound {
    const string& NotFoundHandler::get_name() const noexcept {
        static const string name = "NotFoundHandler";
        return name;
    }

    bool NotFoundHandler::check(shared_ptr<BotHandlerContext> ctx) {
        return ctx->access.full || ctx->access.base;
    }

    ptrMessage NotFoundHandler::handle(shared_ptr<BotHandlerContext> ctx) {
        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = NOT_UNDERSTAND_PHRASE,
            .reply_message_id = ctx->message->id,
        });
    }

}