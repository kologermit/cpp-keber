#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::YouTubeHandler {

struct YouTubeHandler final : InterfaceBotHandler {
    const string& get_name() const noexcept override;
    bool check(shared_ptr<BotHandlerContext> context) override;
    ptrMessage handle(shared_ptr<BotHandlerContext> context) override;

    static ptrMessage to_youtube(shared_ptr<BotHandlerContext> context);
};

}