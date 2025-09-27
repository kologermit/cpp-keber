#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::MenuHandler {

    using std::string_view;
    struct MenuHandler : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> context) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> context) override;
        static ptrMessage to_menu(shared_ptr<BotHandlerContext> context, string_view text = "");
    };
}