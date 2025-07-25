#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <utils/TGBotApi/Types.hpp>
#include <utils/Logger/StdOutLogger.hpp>
#include <utils/Env/Env.hpp>
#include <fmt/core.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>

using json = nlohmann::json;

const std::string BOT_TOKEN = Utils::Env::Get("BOT_TOKEN");
const std::string WEBHOOK_URL = "https://kologermit.ru";
const int PORT = 8083;  // Порт, на котором работает сервер
const std::string API_BASE = "https://api.telegram.org";

using Utils::TGBotApi::Types::Bot;
using Utils::TGBotApi::Types::ptrBot;
using Utils::TGBotApi::Types::Message;
using Utils::TGBotApi::Types::ptrMessage;
using Utils::TGBotApi::Types::ptrBot;
using Utils::TGBotApi::Types::ptrInlineKeyboard;
using Utils::TGBotApi::Types::InlineKeyboard;
using Utils::TGBotApi::Types::InlineButton;
using Utils::TGBotApi::Types::ptrCallbackQuery;
using Utils::TGBotApi::Types::CallbackQuery;
using Utils::Logger::StdOutLogger;
using Utils::Logger::get_logger;
using std::shared_ptr, std::make_shared;
using std::nullopt, std::to_string;
using fmt::format;

int main() {
    const auto logger = Utils::Logger::get_logger(new Utils::Logger::StdOutLogger());
    ptrBot bot = make_shared<Bot>(BOT_TOKEN);
    bot->delete_webhook();
    bot->set_webhook(WEBHOOK_URL + "/webhook");

    logger->info("START_SERVER", format(
        "HOST={} PORT={}",
        "0.0.0.0", PORT
    ));

    // 2. Запускаем сервер для обработки входящих запросов
    httplib::Server().Post("/webhook", [&bot, &logger](const httplib::Request& req, httplib::Response& res) {
        logger->info("REQUEST BODY", req.body);
        json update = json::parse(req.body);
        if (update.contains("message")) {
            ptrMessage update_message = make_shared<Message>(update["message"]);
            logger->info("UPDATE_TEXT", *update_message->get_text());

            auto message = bot->send_message({
                .chat_id = update_message->get_chat()->get_id(),
                .text = update_message->get_text().value_or("(noneo)"),
                .inline_keyboard = ptrInlineKeyboard(new InlineKeyboard({
                    {make_shared<InlineButton>("123", nullopt, "123")}
                }))
            });
        } else if (update.contains("callback_query")) {
            ptrCallbackQuery callback_query = make_shared<CallbackQuery>(update["callback_query"]);
            logger->info("CALLBACK", callback_query->get_data());
            logger->info("CALLBACK_ID", callback_query->get_id());
            logger->info("CALLBACK_ANSWER_RESULT", to_string(bot->answer_callback_query(callback_query->get_id(), nullopt, true)));
        }
        res.status = 200;
    }).Get(".*", [&logger](const httplib::Request& req, httplib::Response& res) {
        res.body = "123";
        res.status = 200;
        logger->info("123", "123");
    }).listen("0.0.0.0", PORT);

    return 0;
}
