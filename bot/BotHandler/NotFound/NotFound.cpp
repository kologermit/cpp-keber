#include <bot/BotHandler/NotFound/NotFound.hpp>
#include <bot/Entity/Repositories.hpp>

namespace Bot::BotHandler::NotFound {

using Bot::Entity::Repositories::get_repositories;
using Utils::TGBotApi::Bot::get_bot;

bool NotFound::check(shared_ptr<BotHandlerContext>) {
    return true;
}

ptrMessage NotFound::handle(shared_ptr<BotHandlerContext> context) {
    return get_bot()->send_message({
        .chat_id = context->chat.telegram_id,
        .text = "Не понял!",
        .reply_message_id = context->message.telegram_id,
    });
}

}