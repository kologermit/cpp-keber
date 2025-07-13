#include <memory>
#include <iostream>
#include <string>
#include <utils/Types.hpp>
#include <utils/Env/Env.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <utils/TGBotApi/Bot/Bot.hpp>
#include <utils/TGBotApi/Query.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/Logger/InterfaceLogger.hpp>
#include <utils/Logger/StdOutLogger.hpp>

using Utils::Logger::GLOBAL_LOGGER;
using Utils::Logger::StdOutLogger;
using Utils::Types::const_string;
using Utils::TGBotApi::Bot::InterfaceBot;
using Utils::TGBotApi::Bot::Bot;
using std::make_shared, std::shared_ptr;
using std::string, std::cout, std::cin, std::to_string;

int main() {
    GLOBAL_LOGGER = make_shared<StdOutLogger>();
    const_string token = Utils::Env::Get("BOT_TOKEN");
    GLOBAL_LOGGER->info("TOKEN", token);
    shared_ptr<InterfaceBot> bot = make_shared<Bot>(token);
    GLOBAL_LOGGER->info("BOT NAME", bot->get_name());
    GLOBAL_LOGGER->info("BOT USERNAME", bot->get_username());
    string message_text; cout << "Input text: "; cin >> message_text;
    const int user_id = 847721936;
    GLOBAL_LOGGER->info("TEXT FROM INPUT", message_text);
    GLOBAL_LOGGER->info("USER ID", to_string(user_id));
    auto message = bot->send_text(user_id, message_text);
    auto chat = message->chat();
    auto from = message->from();
    GLOBAL_LOGGER->info("(q) MESSAGE TEXT", message->get_text());
    GLOBAL_LOGGER->info("(q) MESSAGE ID", to_string(message->get_id()));
    GLOBAL_LOGGER->info("(q) CHAT ID", to_string(chat->get_id()));
    GLOBAL_LOGGER->info("(q) CHAT NAME", chat->get_name());
    GLOBAL_LOGGER->info("(q) FROM ID", to_string(from->get_id()));
    GLOBAL_LOGGER->info("(q) FROM NAME", chat->get_name());
    
}
