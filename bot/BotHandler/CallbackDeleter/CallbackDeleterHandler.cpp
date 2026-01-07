#include <bot/BotHandler/CallbackDeleter/CallbackDeleterHandler.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>

namespace Bot::BotHandler::CallbackDeleter {
    const string& CallbackDeleterHandler::get_name() const noexcept {
        static const string name = "CallbackDeleterHandler";
        return name;
    }

    bool CallbackDeleterHandler::check(shared_ptr<BotHandlerContext> ctx) {
        return ctx->callback != nullptr;
    }

    ptrMessage CallbackDeleterHandler::handle(shared_ptr<BotHandlerContext> ctx) {
        ctx->bot->delete_message(
            ctx->chat->id,
            ctx->message->id
        );
        return nullptr;
    }
}