#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::Access {
    struct AccessCallbackHandler : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> ctx) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> ctx) override;
    };
}