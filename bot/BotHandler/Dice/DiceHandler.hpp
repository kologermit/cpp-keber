#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::Dice {

    using std::string;

    // Команда /d, /dice, /d1-999, /dice1-999
    struct DiceHandler final : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> ctx) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> ctx) override;

        private:

        static bool is_dice_command(const string& text) noexcept;
        static bool is_d_command(const string& text) noexcept;
    };

}