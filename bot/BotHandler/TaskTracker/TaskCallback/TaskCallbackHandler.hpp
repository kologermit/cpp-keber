#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::TaskTracker::TaskCallback {
    struct TaskCallbackHandler : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(ptrContext ctx) override;
        ptrMessage handle(ptrContext ctx) override;
    };
}