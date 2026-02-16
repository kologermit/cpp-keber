#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::TaskTracker::Description {
    struct DescriptionHandler : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(ptrContext ctx) override;
        ptrMessage handle(ptrContext ctx) override;
        static ptrMessage to_task_description(ptrContext ctx, bool save_title = true);
    };
}