#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::TaskTracker::StartAt {
    struct StartAtHandler : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(ptrContext ctx) override;
        ptrMessage handle(ptrContext ctx) override;
        static ptrMessage to_task_start_at(ptrContext ctx, bool save_start_at = true);
    };
}