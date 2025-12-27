#include <bot/BotHandler/NotFound/NotFoundHandler.hpp>
#include <bot/BotHandler/Keys.hpp>

namespace Bot::BotHandler::NotFound {
    const string& NotFoundHandler::get_name() const noexcept {
        static const string name = "NotFoundHandler";
        return name;
    }

    bool NotFoundHandler::check(shared_ptr<BotHandlerContext> context) {
        return context->access.full || context->access.base;
    }

    ptrMessage NotFoundHandler::handle(shared_ptr<BotHandlerContext> context) {
        return context->bot->send_message({
            .chat_id = context->chat->id,
            .text = NOT_ANDERSTAND_PHRASE,
            .reply_message_id = context->message->id,
        });
    }

}