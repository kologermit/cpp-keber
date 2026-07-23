#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <string_view>
#include <string>

namespace Bot::BotHandler::PatchTask {
    using std::string_view;
    using std::string;
    using Utils::TGBotApi::Types::ptrMessage;
    using Bot::BotHandler::InterfaceBotHandler;

    struct PatchTaskHandler : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(ptrContext ctx) override;
        ptrMessage handle(ptrContext ctx) override;
    };
}