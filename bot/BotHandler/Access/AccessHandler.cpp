#include <bot/BotHandler/Access/AccessHandler.hpp>
#include <bot/BotHandler/Menu/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <fmt/core.h>
#include <map>
#include <algorithm>
#include <cctype>

namespace Bot::BotHandler::Access {

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
    using Bot::BotHandler::Menu::MenuHandler;
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

    bool AccessHandler::check(shared_ptr<BotHandlerContext> ctx) {
        return (ctx->access.full || ctx->access.access)
        && ctx->callback == nullptr
        && ctx->user->screen == ACCESS
        && ctx->message->text.size() <= 20
        && (
            ctx->message->text.starts_with("@")
            || all_of(ctx->message->text, [](char c) {return isdigit(c);})
            || ctx->message->text == BACK_WORD
        );
    }

    ptrMessage AccessHandler::handle(shared_ptr<BotHandlerContext> ctx) {
        if (ctx->message->text == BACK_WORD) {
            return MenuHandler::to_menu(ctx);
        }
        unique_ptr<User> find_user = nullptr;
        if (ctx->message->text.starts_with("@")) {
            find_user = ctx->db->user->get_by_username(ctx->message->text.substr(1));
        } else {
            find_user = ctx->db->user->get_by_id(stoi(ctx->message->text));
        }
        if (find_user == nullptr) {
            return ctx->bot->send_message( {
                .chat_id = ctx->chat->id,
                .text = format(USER_NOT_FOUND_PHRASE, ctx->message->text),
                .reply_message_id = ctx->message->id,
            });
        }
        const auto find_user_access = ctx->db->access->get_by_user_id(find_user->id);
        return send_message_with_access_keyboard(ctx, *find_user, find_user_access);
    }

    ptrMessage AccessHandler::send_message_with_access_keyboard(
        shared_ptr<BotHandlerContext> ctx,
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
        return ctx->bot->send_message( {
            .chat_id = ctx->chat->id,
            .text = format(USER_TEMPLATE,
                    user.name,
                    user.id,
                    "@" + user.username
                ),
            .reply_message_id = ctx->message->id,
            .inline_keyboard = make_unique<InlineKeyboard>(std::move(buttons))
        });
    }

    ptrMessage AccessHandler::to_access(shared_ptr<BotHandlerContext> ctx) {
        string access_answer = "";
        for (auto&[fst, snd] : map<const char*, bool>{
            {BASE_WORD, ctx->access.base},
            {FULL_WORD, ctx->access.full},
            {ACCESS_WORD, ctx->access.access},
            {YOUTUBE_WORD, ctx->access.youtube},
            {TASK_WORD, ctx->access.task},
            {DOCKER_WORD, ctx->access.docker},
            {SERVER_WORD, ctx->access.server},
        }) {
            if (snd) {
                access_answer += string(fst) + " ";
            }
        }

        ctx->user->screen = ACCESS;
        ctx->db->user->update(*ctx->user);

        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = format(ACCESS_PHRASE, access_answer),
            .reply_message_id = ctx->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{
                {make_shared<ReplyButton>(BACK_WORD)}
            })
        });
    }
}