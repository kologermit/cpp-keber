#include <bot/BotHandler/Start/StartHandler.hpp>
#include <bot/BotHandler/Menu/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <set>

namespace Bot::BotHandler::Start {
    using Entity::User::EnumUserScreen;
    using Entity::User::EnumUserScreen;
    using Bot::BotHandler::Menu::MenuHandler;
    using std::set;

    const string& StartHandler::get_name() const noexcept {
        static const string name = "StartHandler";
        return name;
    }

    bool StartHandler::check(shared_ptr<BotHandlerContext> context) {
        static const set back_screens{
            EnumUserScreen::START,
            EnumUserScreen::MENU,
        };
        return
            (context->access.full || context->access.base)
            && (
                context->message->text.starts_with(START_COMMAND)
                || context->message->text.starts_with(MENU_COMMAND)
                || context->user->screen == EnumUserScreen::START
                || (context->message->text == BACK_WORD && back_screens.contains(context->user->screen))
            );
    }

    ptrMessage StartHandler::handle(shared_ptr<BotHandlerContext> context) {
        return MenuHandler::to_menu(context,
            context->message->text.starts_with(START_COMMAND) ?
            START_PHRASE : ""
        );
    }

}