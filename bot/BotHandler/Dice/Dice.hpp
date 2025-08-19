#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::Dice {

using std::string;

// Команда /d, /dice, /d1-999, /dice1-999
struct Dice : InterfaceBotHandler {
    bool check(shared_ptr<BotHandlerContext> context) override;
    ptrMessage handle(shared_ptr<BotHandlerContext> context) override;

    private:

    bool is_dice_command(const string& text) const noexcept;
    bool is_d_command(const string& text) const noexcept;
};

}