#include <bot/BotHandler/Start/StartHandler.hpp>
#include <bot/BotHandler/Menu/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <set>

namespace Bot::BotHandler::Start {
    using Entity::User::UserScreen;
    using Entity::User::UserScreen;
    using Bot::BotHandler::Menu::MenuHandler;
    using std::set;

    const string& StartHandler::get_name() const noexcept {
        static const string name = "StartHandler";
        return name;
    }

    bool StartHandler::check(shared_ptr<BotHandlerContext> ctx) {
        static const set back_screens{
            UserScreen::START,
            UserScreen::MENU,
        };
        return
            (ctx->access.full || ctx->access.base)
            && (
                ctx->message->text.starts_with(START_COMMAND)
                || ctx->message->text.starts_with(MENU_COMMAND)
                || ctx->user->screen == UserScreen::START
                || (ctx->message->text == BACK_WORD && back_screens.contains(ctx->user->screen))
            );
    }

    ptrMessage StartHandler::handle(shared_ptr<BotHandlerContext> ctx) {
        return MenuHandler::to_menu(ctx,
            ctx->message->text.starts_with(START_COMMAND) ?
            START_PHRASE : ""
        );
    }

}