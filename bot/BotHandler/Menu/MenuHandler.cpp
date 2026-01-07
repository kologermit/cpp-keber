#include <bot/BotHandler/Menu/MenuHandler.hpp>
#include <bot/BotHandler/Access/AccessHandler.hpp>
#include <bot/BotHandler/YouTube/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <algorithm>

namespace Bot::BotHandler::Menu {
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::ReplyButtons;
    using Bot::BotHandler::Access::AccessHandler;
    using Bot::BotHandler::YouTube::YouTubeHandler;
    using Entity::User::MENU;
    using std::make_unique;
    using std::make_shared;
    using std::map;
    using std::pair;
    using std::set;

    const string& MenuHandler::get_name() const noexcept {
        static const string& name = "MenuHandler";
        return name;
    }

    bool MenuHandler::check(shared_ptr<BotHandlerContext> ctx) {
        static const set<string> words{
            ACCESS_WORD,
            TASK_WORD,
            YOUTUBE_WORD,
            DOCKER_WORD,
            SERVER_WORD,
        };
        return
            (ctx->access.full || ctx->access.base)
            && ctx->user->screen == MENU
            && words.contains(ctx->message->text);
    }

    ptrMessage MenuHandler::handle(shared_ptr<BotHandlerContext> ctx) {
        if (ctx->message->text == ACCESS_WORD) {
            return AccessHandler::to_access(ctx);
        }

        if (ctx->message->text == YOUTUBE_WORD) {
            return YouTubeHandler::to_youtube(ctx);
        }

        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = IN_DEVELOP_PHRASE,
            .reply_message_id = ctx->message->id,
        });
    }

    ptrMessage MenuHandler::to_menu(shared_ptr<BotHandlerContext> ctx, string_view text) {
        ctx->user->screen = MENU;
        ctx->db->user->update(*ctx->user);

        ReplyButtons buttons(3, vector<shared_ptr<ReplyButton> >(3, nullptr));

        const map<pair<size_t, size_t>, pair<bool, string_view> > buttons_with_access{
            {{0, 0}, {ctx->access.access, ACCESS_WORD}},
            {{1, 0}, {ctx->access.task, TASK_WORD}},
            {{1, 1}, {ctx->access.youtube, YOUTUBE_WORD}},
            {{2, 0}, {ctx->access.docker, DOCKER_WORD}},
            {{2, 1}, {ctx->access.server, SERVER_WORD}},
        };

        for (const auto&[fst, snd] : buttons_with_access) {
            const auto [button_x, button_y] = fst;
            const auto [button_access, button_text] = snd;

            if (ctx->access.full || button_access) {
                buttons[button_x][button_y] = make_shared<ReplyButton>(button_text);
            }
        }

        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = (text.empty() ? MENU_WORD : text.data()),
            .reply_message_id = ctx->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(std::move(buttons))
        });
    }
}