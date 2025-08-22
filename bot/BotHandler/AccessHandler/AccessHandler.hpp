#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::AccessHandler {

struct AccessHandler : InterfaceBotHandler {
    const string& get_name() const noexcept override;
    bool check(shared_ptr<BotHandlerContext> context) override;
    ptrMessage handle(shared_ptr<BotHandlerContext> context) override;

    static ptrMessage to_access(shared_ptr<BotHandlerContext> context);
};

}