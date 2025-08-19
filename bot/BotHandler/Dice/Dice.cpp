#include <bot/BotHandler/Dice/Dice.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/Random/Random.hpp>
#include <fmt/core.h>
#include <regex>
#include <string>

#include <iostream>

namespace Bot::BotHandler::Dice {

using Utils::TGBotApi::Bot::get_bot;
using Utils::Random::rand_int;
using fmt::format;
using std::regex;
using std::regex_match;
using std::stoi;
using std::string;

const string DICE_COMMAND = "/dice";
const string D_COMMAND = "/d";

bool Dice::is_dice_command(const string& text) const noexcept {
    static const regex pattern("^\\" + DICE_COMMAND + "(\\d{0,6})$");
    return regex_match(text, pattern);
}

bool Dice::is_d_command(const string& text) const noexcept {
    static const regex pattern("^\\" + D_COMMAND + "(\\d{0,6})$");
    return regex_match(text, pattern);
}

bool Dice::check(shared_ptr<BotHandlerContext> context) {
    return is_dice_command(context->message.text) || is_d_command(context->message.text);
}

ptrMessage Dice::handle(shared_ptr<BotHandlerContext> context) {
    int dice = 20;

    if (is_dice_command(context->message.text) && context->message.text != DICE_COMMAND) {
        dice = stoi(context->message.text.substr(DICE_COMMAND.size()));
    }

    if (is_d_command(context->message.text) && context->message.text != D_COMMAND) {
        dice = stoi(context->message.text.substr(D_COMMAND.size()));
    }

    return get_bot()->send_message({
        .chat_id = context->chat.telegram_id,
        .text = format(
            "Бросок кубика на <b>{}</b>\nРезультат: <b>{}</b>",
            dice,
            rand_int(1, dice)
        ),
        .reply_message_id = context->message.telegram_id,
    });
}

}