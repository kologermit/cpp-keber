#include <bot/BotHandler/CallbackDeleter/CallbackDeleterHandler.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>

namespace Bot::BotHandler::CallbackDeleter {
    const string& CallbackDeleterHandler::get_name() const noexcept {
        static const string name = "CallbackDeleterHandler";
        return name;
    }

    bool CallbackDeleterHandler::check(shared_ptr<BotHandlerContext> context) {
        return context->callback != nullptr;
    }

    ptrMessage CallbackDeleterHandler::handle(shared_ptr<BotHandlerContext> context) {
        context->bot->delete_message(
            context->chat->id,
            context->message->id
        );
        return nullptr;
    }
}