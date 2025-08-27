#include <bot/BotHandler/AccessHandler/AccessHandler.hpp>
#include <bot/BotHandler/MenuHandler/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <fmt/core.h>
#include <map>
#include <algorithm>
#include <cctype>

namespace Bot::BotHandler::AccessHandler {

using std::make_unique;
using std::make_shared;
using std::map;
using std::ranges::all_of;
using std::isdigit;
using std::unique_ptr;
using std::stoi;
using fmt::format;
using Entity::User::ACCESS;
using Entity::User::SCREEN;
using Entity::User::User;
using Entity::Repositories::get_repositories;
using MenuHandler::MenuHandler;
using Utils::TGBotApi::Bot::get_bot;
using Utils::TGBotApi::Types::ReplyKeyboard;
using Utils::TGBotApi::Types::ReplyButtons;
using Utils::TGBotApi::Types::ReplyButton;

const string& AccessHandler::get_name() const noexcept {
    static const string& name = "AccessHandler";
    return name;
}

bool AccessHandler::check(shared_ptr<BotHandlerContext> context) {
    return (context->access.full || context->access.access)
    && context->user->screen == ACCESS
    && context->message->text.size() <= 20
    && (context->message->text.starts_with("@") ||
        all_of(context->message->text, [](char c) {return isdigit(c);}));
}

ptrMessage AccessHandler::handle(shared_ptr<BotHandlerContext> context) {
    unique_ptr<User> find_user = nullptr;
    if (context->message->text.starts_with("@")) {
        find_user = get_repositories()->user_repository->get_by_username(context->message->text.substr(1));
    } else {
        find_user = get_repositories()->user_repository->get_by_telegram_id(stoi(context->message->text));
    }
    if (find_user == nullptr) {
        return get_bot()->send_message( {
            .chat_id = context->chat->telegram_id,
            .text = format(USER_NOT_FOUND_PHRASE, context->message->text),
            .reply_message_id = context->message->telegram_id,
        });
    }
    ReplyButtons buttons;
    auto find_user_access = get_repositories()->access_repository->get_by_user_id(find_user->id);
    for (auto&[fst, snd] : map<const char*, bool>{
        {BASE_WORD, find_user_access.base},
        {FULL_WORD, find_user_access.full},
        {ACCESS_WORD, find_user_access.access},
        {YOUTUBE_WORD, find_user_access.youtube},
        {TASK_WORD, find_user_access.task},
        {DOCKER_WORD, find_user_access.docker},
        {SERVER_WORD, find_user_access.server},
    }) {
        auto button = make_shared<ReplyButton>(format("{} {}", fst, (snd ? SUCCESS_SYMBOL : ERROR_SYMBOL)));
        buttons.push_back(vector<shared_ptr<ReplyButton>>(1, button));
    }
    return get_bot()->send_message( {
        .chat_id = context->chat->telegram_id,
        .text = format(USER_TEMPLATE,
                find_user->name,
                find_user->id,
                find_user->telegram_id,
                "@" + find_user->username
            ),
        .reply_message_id = context->message->telegram_id,
        .reply_keyboard = make_unique<ReplyKeyboard>(move(buttons))
    });
}

ptrMessage AccessHandler::to_access(shared_ptr<BotHandlerContext> context) {
    string access_answer = "";
    for (auto&[fst, snd] : map<const char*, bool>{
        {BASE_WORD, context->access.base},
        {FULL_WORD, context->access.full},
        {ACCESS_WORD, context->access.access},
        {YOUTUBE_WORD, context->access.youtube},
        {TASK_WORD, context->access.task},
        {DOCKER_WORD, context->access.docker},
        {SERVER_WORD, context->access.server},
    }) {
        if (snd) {
            access_answer += string(fst) + " ";
        }
    }

    context->user->screen = ACCESS;
    get_repositories()->user_repository->update(*context->user, {SCREEN});

    return get_bot()->send_message({
        .chat_id = context->chat->telegram_id,
        .text = format(ACCESS_PHRASE, access_answer),
        .reply_message_id = context->message->telegram_id,
        .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{
            {make_shared<ReplyButton>(BACK_WORD)}
        })
    });
}

}