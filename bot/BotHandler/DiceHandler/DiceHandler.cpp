#include <bot/BotHandler/DiceHandler/DiceHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/Random/Random.hpp>
#include <fmt/core.h>
#include <regex>
#include <string>

namespace Bot::BotHandler::DiceHandler {

using Utils::TGBotApi::Bot::get_bot;
using Utils::Random::rand_int;
using fmt::format;
using std::regex;
using std::regex_match;
using std::stoi;
using std::string;

const string DICE_COMMAND = "/dice";
const string D_COMMAND = "/d";

bool DiceHandler::is_dice_command(const string& text) noexcept {
    static const regex pattern("^\\" + DICE_COMMAND + "(\\d{0,6})$");
    return regex_match(text, pattern);
}

bool DiceHandler::is_d_command(const string& text) noexcept {
    static const regex pattern("^\\" + D_COMMAND + "(\\d{0,6})$");
    return regex_match(text, pattern);
}

const string& DiceHandler::get_name() const noexcept {
    static const string name = "DiceHandler";
    return name;
}

bool DiceHandler::check(shared_ptr<BotHandlerContext> context) {
    return 
    (context->access.full || context->access.base)
    && (is_dice_command(context->message->text) || is_d_command(context->message->text));
}

ptrMessage DiceHandler::handle(shared_ptr<BotHandlerContext> context) {
    int dice = 20;

    if (is_dice_command(context->message->text) && context->message->text != DICE_COMMAND) {
        dice = stoi(context->message->text.substr(DICE_COMMAND.size()));
    }

    if (is_d_command(context->message->text) && context->message->text != D_COMMAND) {
        dice = stoi(context->message->text.substr(D_COMMAND.size()));
    }

    return get_bot()->send_message({
        .chat_id = context->chat->telegram_id,
        .text = format(
            DICE_PHRASE,
            dice,
            rand_int(1, dice)
        ),
        .reply_message_id = context->message->telegram_id,
    });
}

}