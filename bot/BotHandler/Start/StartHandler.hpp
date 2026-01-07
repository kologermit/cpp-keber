#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::Start {

    struct StartHandler : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> ctx) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> ctx) override;
    };

}