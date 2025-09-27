#include <bot/BotHandler/CallbackDeleterHandler/CallbackDeleterHandler.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>

namespace Bot::BotHandler::CallbackDeleterHandler {

    using Utils::TGBotApi::Bot::get_bot;

    const string& CallbackDeleterHandler::get_name() const noexcept {
        static const string name = string("CallbackDeleterHandler");
        return name;
    }

    bool CallbackDeleterHandler::check(shared_ptr<BotHandlerContext> context) {
        return context->callback != nullptr;
    }

    ptrMessage CallbackDeleterHandler::handle(shared_ptr<BotHandlerContext> context) {
        get_bot()->delete_message(
            context->chat->id,
            context->message->id
        );
        return nullptr;
    }
}