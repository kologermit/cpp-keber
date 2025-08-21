#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::Menu {

using std::string_view;

struct Menu : InterfaceBotHandler {

    bool check(shared_ptr<BotHandlerContext> context) override;
    ptrMessage handle(shared_ptr<BotHandlerContext> context) override;
    static ptrMessage to_menu(shared_ptr<BotHandlerContext> context, string_view text = "");

};



}