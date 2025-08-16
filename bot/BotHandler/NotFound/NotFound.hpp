#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::NotFound {

struct NotFound : InterfaceBotHandler {
    bool check(shared_ptr<BotHandlerContext> context) override;
    ptrMessage handle(shared_ptr<BotHandlerContext> context) override;
};

}