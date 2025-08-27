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
using AccessHandler::AccessHandler;
using Entity::Repositories::get_repositories;
using Entity::User::MENU;
using Entity::User::SCREEN;
using std::make_unique;
using std::make_shared;
using std::move;
using std::map;
using std::pair;

const string& MenuHandler::get_name() const noexcept {
    static const string& name = "MenuHandler";
    return name;
}

bool MenuHandler::check(shared_ptr<BotHandlerContext> context) {
    return (context->access.full || context->access.base) && context->user->screen == MENU;
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

    ReplyButtons buttons(3, vector<shared_ptr<ReplyButton> >(3, nullptr));

    const map<pair<size_t, size_t>, pair<bool, string_view> > buttons_with_access{
        {{0, 0}, {context->access.access, ACCESS_WORD}},
        {{0, 1}, {context->access.task, TASK_WORD}},
        {{1, 1}, {context->access.youtube, YOUTUBE_WORD}},
        {{0, 2}, {context->access.docker, DOCKER_WORD}},
        {{1, 2}, {context->access.server, SERVER_WORD}},
    };

    for (const auto& button : buttons_with_access) {
        size_t button_x = button.first.first;
        size_t button_y = button.first.second;
        bool button_access = button.second.first;
        string_view button_text = button.second.second;

        if (context->access.full || button_access) {
            buttons[button_x][button_y] = make_shared<ReplyButton>(button_text);
        }
    }

    return get_bot()->send_message({
        .chat_id = context->chat->telegram_id,
        .text = (text.empty() ? MENU_WORD : text.data()),
        .reply_message_id = context->message->telegram_id,
        .reply_keyboard = make_unique<ReplyKeyboard>(move(buttons))
    });
}

}