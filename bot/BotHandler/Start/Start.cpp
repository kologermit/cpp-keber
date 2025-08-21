#include <bot/BotHandler/Start/Start.hpp>
#include <bot/BotHandler/Menu/Menu.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>

namespace Bot::BotHandler::Start {

using Utils::TGBotApi::Bot::get_bot;
using Bot::Entity::User::START;
using Bot::Entity::Repositories::get_repositories;
using Bot::BotHandler::Menu::Menu;

constexpr const char* START_COMMAND = "/start";

bool Start::check(shared_ptr<BotHandlerContext> context) {
    return 
        context->message.text.starts_with(START_COMMAND)
        || context->user.screen == START;
}

ptrMessage Start::handle(shared_ptr<BotHandlerContext> context) {
    return Menu::to_menu(context, "Приветствую в CPP-KEBER!");
}

}