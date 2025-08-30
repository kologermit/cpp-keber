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
    using std::to_string;
    using std::pair;
    using fmt::format;
    using nlohmann::json;
    using Entity::Access::EnumAccessType;
    using Entity::User::ACCESS;
    using Entity::User::SCREEN;
    using Entity::User::User;
    using Entity::Repositories::get_repositories;
    using MenuHandler::MenuHandler;
    using Utils::Entity::ID_COLUMN;
    using Utils::TGBotApi::Bot::get_bot;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButtons;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::ReplyLane;
    using Utils::TGBotApi::Types::InlineKeyboard;
    using Utils::TGBotApi::Types::InlineButtons;
    using Utils::TGBotApi::Types::InlineButton;
    using Utils::TGBotApi::Types::InlineLane;

    const string& AccessHandler::get_name() const noexcept {
        static const string& name = "AccessHandler";
        return name;
    }

    bool AccessHandler::check(shared_ptr<BotHandlerContext> context) {
        return (context->access.full || context->access.access)
        && context->callback == nullptr
        && context->user->screen == ACCESS
        && context->message->text.size() <= 20
        && (
            context->message->text.starts_with("@")
            || all_of(context->message->text, [](char c) {return isdigit(c);})
            || context->message->text == BACK_WORD
        );
    }

    ptrMessage AccessHandler::handle(shared_ptr<BotHandlerContext> context) {
        if (context->message->text == BACK_WORD) {
            return MenuHandler::to_menu(context);
        }
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
        auto find_user_access = get_repositories()->access_repository->get_by_user_id(find_user->id);
        return send_message_with_access_keyboard(context, *find_user, find_user_access);
    }

    ptrMessage AccessHandler::send_message_with_access_keyboard(
        shared_ptr<BotHandlerContext> context,
        const User& user,
        UserAccess user_access
    ) {
        InlineButtons buttons;
        for (auto&[word, data] : map<const char*, pair<EnumAccessType, bool> >{
            {BASE_WORD, {EnumAccessType::BASE, user_access.base}},
            {FULL_WORD, {EnumAccessType::FULL, user_access.full}},
            {ACCESS_WORD, {EnumAccessType::ACCESS, user_access.access}},
            {YOUTUBE_WORD, {EnumAccessType::YOUTUBE, user_access.youtube}},
            {TASK_WORD, {EnumAccessType::TASK, user_access.task}},
            {DOCKER_WORD, {EnumAccessType::DOCKER, user_access.docker}},
            {SERVER_WORD, {EnumAccessType::SERVER, user_access.server}},
        }) {
            auto [access_type, access_value] = data;
            auto button = make_shared<InlineButton>(
                format("{} {}", word, (access_value ? SUCCESS_SYMBOL : ERROR_SYMBOL)),
                "",
                json{
                    ACCESS_CALLBACK_NAME,
                    user.id,
                    access_type,
                    !access_value,
                    word,
                }.dump()
            );
            buttons.push_back(InlineLane(1, button));
        }
        return get_bot()->send_message( {
            .chat_id = context->chat->telegram_id,
            .text = format(USER_TEMPLATE,
                    user.name,
                    user.id,
                    user.telegram_id,
                    "@" + user.username
                ),
            .reply_message_id = context->message->telegram_id,
            .inline_keyboard = make_unique<InlineKeyboard>(move(buttons))
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

        User user;
        user.id = context->user->id;
        user.screen = ACCESS;
        get_repositories()->user_repository->update(user, {SCREEN});

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