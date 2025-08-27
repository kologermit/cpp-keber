#include <bot/BotHandler/StartHandler/StartHandler.hpp>
#include <bot/BotHandler/MenuHandler/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>

namespace Bot::BotHandler::StartHandler {

using Utils::TGBotApi::Bot::get_bot;
using Bot::Entity::User::START;
using Bot::Entity::Repositories::get_repositories;
using Bot::BotHandler::MenuHandler::MenuHandler;

const string& StartHandler::get_name() const noexcept {
    static const string name = "StartHandler";
    return name;
}

bool StartHandler::check(shared_ptr<BotHandlerContext> context) {
    return 
        (context->access.full || context->access.base)
        && (
            context->message->text.starts_with(START_COMMAND)
            || context->message->text.starts_with(MENU_COMMAND)
            || context->user->screen == START
        );
}

ptrMessage StartHandler::handle(shared_ptr<BotHandlerContext> context) {
    return MenuHandler::to_menu(context,
        context->message->text.starts_with(START_COMMAND) ?
        START_PHRASE : ""
    );
}

}