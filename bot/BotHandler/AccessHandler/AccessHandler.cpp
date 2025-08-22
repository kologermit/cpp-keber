#include <bot/BotHandler/AccessHandler/AccessHandler.hpp>
#include <bot/BotHandler/MenuHandler/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>

namespace Bot::BotHandler::AccessHandler {

using std::make_unique;
using Bot::Entity::User::ACCESS;
using Bot::Entity::User::SCREEN;
using Bot::Entity::Repositories::get_repositories;
using Bot::BotHandler::MenuHandler::MenuHandler;
using Utils::TGBotApi::Bot::get_bot;
using Utils::TGBotApi::Types::ReplyKeyboard;

const string& AccessHandler::get_name() const noexcept {
    static const string& name = "AccessHandler";
    return name;
}

bool AccessHandler::check(shared_ptr<BotHandlerContext> context) {
    return context->user->screen == ACCESS;
}

ptrMessage AccessHandler::handle(shared_ptr<BotHandlerContext> context) {
    return MenuHandler::to_menu(context, ACCESS_WORD);
}

ptrMessage AccessHandler::to_access(shared_ptr<BotHandlerContext> context) {
    context->user->screen = ACCESS;
    get_repositories()->user_repository->update(*context->user, {SCREEN});
    return get_bot()->send_message({
        .chat_id = context->chat->telegram_id,
        .text = ACCESS_WORD,
        .reply_message_id = context->message->telegram_id,
    });
}

}