#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::Access {

constexpr const char* ACCESS_CALLBACK_NAME = "ac";

struct AccessHandler final : InterfaceBotHandler {
    const string& get_name() const noexcept override;
    bool check(shared_ptr<BotHandlerContext> ctx) override;
    ptrMessage handle(shared_ptr<BotHandlerContext> ctx) override;

    static ptrMessage to_access(shared_ptr<BotHandlerContext> ctx);
    static ptrMessage send_message_with_access_keyboard(
        shared_ptr<BotHandlerContext> ctx,
        const User& user,
        UserAccess user_access
    );
};

}