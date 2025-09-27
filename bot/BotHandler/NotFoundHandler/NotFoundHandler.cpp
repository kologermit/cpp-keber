#include <bot/BotHandler/NotFoundHandler/NotFoundHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>

namespace Bot::BotHandler::NotFoundHandler {

    using Bot::Entity::Repositories::get_repositories;
    using Utils::TGBotApi::Bot::get_bot;

    const string& NotFoundHandler::get_name() const noexcept {
        static const string name = "NotFoundHandler";
        return name;
    }

    bool NotFoundHandler::check(shared_ptr<BotHandlerContext> context) {
        return context->access.full || context->access.base;
    }

    ptrMessage NotFoundHandler::handle(shared_ptr<BotHandlerContext> context) {
        return get_bot()->send_message({
            .chat_id = context->chat->id,
            .text = NOT_ANDERSTAND_PHRASE,
            .reply_message_id = context->message->id,
        });
    }

}