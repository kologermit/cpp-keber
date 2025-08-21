#include <bot/BotHandler/Menu/Menu.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>

namespace Bot::BotHandler::Menu {

using Utils::TGBotApi::Bot::get_bot;
using Utils::TGBotApi::Types::ReplyKeyboard;
using Utils::TGBotApi::Types::ReplyButton;
using Utils::TGBotApi::Types::ReplyButtons;
using Bot::Entity::Repositories::get_repositories;
using Bot::Entity::User::MENU;
using Bot::Entity::User::SCREEN;
using std::make_unique;
using std::make_shared;

bool Menu::check(shared_ptr<BotHandlerContext> context) {
    return context->user.screen == MENU;
}

ptrMessage Menu::handle(shared_ptr<BotHandlerContext> context) {
    return get_bot()->send_message({
        .chat_id = context->chat.telegram_id,
        .text = "Обработчик Меню",
        .reply_message_id = context->message.telegram_id,
    });
}

ptrMessage Menu::to_menu(shared_ptr<BotHandlerContext> context, string_view text) {
    context->user.screen = MENU;
    get_repositories()->user_repository->update(context->user, {SCREEN});
    return get_bot()->send_message({
        .chat_id = context->chat.telegram_id,
        .text = (text.empty() ? "Меню" : text.data()),
        .reply_message_id = context->message.telegram_id,
        .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{
            {make_shared<ReplyButton>("Задачи"), make_shared<ReplyButton>("Скачать")}
        })
    });
}

}