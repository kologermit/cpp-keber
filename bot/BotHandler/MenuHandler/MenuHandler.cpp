#include <bot/BotHandler/MenuHandler/MenuHandler.hpp>
#include <bot/BotHandler/AccessHandler/AccessHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>

namespace Bot::BotHandler::MenuHandler {

using Utils::TGBotApi::Bot::get_bot;
using Utils::TGBotApi::Types::ReplyKeyboard;
using Utils::TGBotApi::Types::ReplyButton;
using Utils::TGBotApi::Types::ReplyButtons;
using Bot::BotHandler::AccessHandler::AccessHandler;
using Bot::Entity::Repositories::get_repositories;
using Bot::Entity::User::MENU;
using Bot::Entity::User::SCREEN;
using std::make_unique;
using std::make_shared;

const string& MenuHandler::get_name() const noexcept {
    static const string& name = "MenuHandler";
    return name;
}

bool MenuHandler::check(shared_ptr<BotHandlerContext> context) {
    return context->user->screen == MENU;
}

ptrMessage MenuHandler::handle(shared_ptr<BotHandlerContext> context) {
    if (context->message->text == ACCESS_WORD) {
        return AccessHandler::to_access(context);
    }
    return get_bot()->send_message({
        .chat_id = context->chat->telegram_id,
        .text = MENU_WORD,
        .reply_message_id = context->message->telegram_id,
    });
}

ptrMessage MenuHandler::to_menu(shared_ptr<BotHandlerContext> context, string_view text) {
    context->user->screen = MENU;
    get_repositories()->user_repository->update(*context->user, {SCREEN});
    return get_bot()->send_message({
        .chat_id = context->chat->telegram_id,
        .text = (text.empty() ? MENU_WORD : text.data()),
        .reply_message_id = context->message->telegram_id,
        .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{
            {make_shared<ReplyButton>(ACCESS_WORD)}
        })
    });
}

}