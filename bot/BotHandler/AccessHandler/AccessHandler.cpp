#include <bot/BotHandler/AccessHandler/AccessHandler.hpp>
#include <bot/BotHandler/MenuHandler/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <fmt/core.h>
#include <map>

namespace Bot::BotHandler::AccessHandler {

using std::make_unique;
using std::make_shared;
using std::map;
using fmt::format;
using Bot::Entity::User::ACCESS;
using Bot::Entity::User::SCREEN;
using Bot::Entity::Repositories::get_repositories;
using Bot::BotHandler::MenuHandler::MenuHandler;
using Utils::TGBotApi::Bot::get_bot;
using Utils::TGBotApi::Types::ReplyKeyboard;
using Utils::TGBotApi::Types::ReplyButtons;
using Utils::TGBotApi::Types::ReplyButton;

const string& AccessHandler::get_name() const noexcept {
    static const string& name = "AccessHandler";
    return name;
}

bool AccessHandler::check(shared_ptr<BotHandlerContext> context) {
    return
    (context->access.full || context->access.access) 
    && context->user->screen == ACCESS;
}

ptrMessage AccessHandler::handle(shared_ptr<BotHandlerContext> context) {
    return MenuHandler::to_menu(context, ACCESS_WORD);
}

ptrMessage AccessHandler::to_access(shared_ptr<BotHandlerContext> context) {
    string access_answer = "";
    for (auto& word_access : map<const char*, bool>{
        {BASE_WORD, context->access.base},
        {FULL_WORD, context->access.full},
        {ACCESS_WORD, context->access.access},
        {YOUTUBE_WORD, context->access.youtube},
        {TASK_WORD, context->access.task},
        {DOCKER_WORD, context->access.docker},
        {SERVER_WORD, context->access.server},
    }) {
        if (word_access.second) {
            access_answer += string(word_access.first) + " ";
        }
    }

    context->user->screen = ACCESS;
    get_repositories()->user_repository->update(*context->user, {SCREEN});

    return get_bot()->send_message({
        .chat_id = context->chat->telegram_id,
        .text = format(ACCESS_PHRASE, access_answer),
        .reply_message_id = context->message->telegram_id,
    });
}

}