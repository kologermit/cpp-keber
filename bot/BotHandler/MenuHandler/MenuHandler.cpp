#include <bot/BotHandler/MenuHandler/MenuHandler.hpp>
#include <bot/BotHandler/AccessHandler/AccessHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <algorithm>

namespace Bot::BotHandler::MenuHandler {

using Utils::TGBotApi::Bot::get_bot;
using Utils::TGBotApi::Types::ReplyKeyboard;
using Utils::TGBotApi::Types::ReplyButton;
using Utils::TGBotApi::Types::ReplyButtons;
using AccessHandler::AccessHandler;
using Entity::Repositories::get_repositories;
using Entity::User::User;
using Entity::User::MENU;
using Entity::User::SCREEN;
using std::make_unique;
using std::make_shared;
using std::move;
using std::map;
using std::pair;
using std::set;

const string& MenuHandler::get_name() const noexcept {
    static const string& name = "MenuHandler";
    return name;
}

bool MenuHandler::check(shared_ptr<BotHandlerContext> context) {
    static const set<string> words{
        ACCESS_WORD,
        TASK_WORD,
        YOUTUBE_WORD,
        DOCKER_WORD,
        SERVER_WORD,
    };
    return
        (context->access.full || context->access.base)
        && context->user->screen == MENU
        && words.find(context->message->text) != words.end();
}

ptrMessage MenuHandler::handle(shared_ptr<BotHandlerContext> context) {
    if (context->message->text == ACCESS_WORD) {
        return AccessHandler::to_access(context);
    }

    return get_bot()->send_message({
        .chat_id = context->chat->telegram_id,
        .text = IN_DEVELOP_PHRASE,
        .reply_message_id = context->message->telegram_id,
    });
}

ptrMessage MenuHandler::to_menu(shared_ptr<BotHandlerContext> context, string_view text) {
    User user;
    user.id = context->user->id;
    user.screen = MENU;
    get_repositories()->user_repository->update(user, {SCREEN});

    ReplyButtons buttons(3, vector<shared_ptr<ReplyButton> >(3, nullptr));

    const map<pair<size_t, size_t>, pair<bool, string_view> > buttons_with_access{
        {{0, 0}, {context->access.access, ACCESS_WORD}},
        {{1, 0}, {context->access.task, TASK_WORD}},
        {{1, 1}, {context->access.youtube, YOUTUBE_WORD}},
        {{2, 0}, {context->access.docker, DOCKER_WORD}},
        {{2, 1}, {context->access.server, SERVER_WORD}},
    };

    for (const auto&[fst, snd] : buttons_with_access) {
        const auto [button_x, button_y] = fst;
        const auto [button_access, button_text] = snd;

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