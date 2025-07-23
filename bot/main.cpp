#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <utils/TGBotApi/Bot/Bot.hpp>
#include <utils/TGBotApi/Message/Message.hpp>
#include <utils/TGBotApi/Message/Keyboard/InlineKeyboard.hpp>
#include <utils/TGBotApi/Message/Keyboard/InlineButton.hpp>
#include <utils/Logger/StdOutLogger.hpp>
#include <utils/Env/Env.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <thread>

using json = nlohmann::json;

const std::string BOT_TOKEN = Utils::Env::Get("BOT_TOKEN");
const std::string WEBHOOK_URL = "https://kologermit.ru";
const int PORT = 8083;  // Порт, на котором работает сервер
const std::string API_BASE = "https://api.telegram.org";

using Utils::TGBotApi::Bot::Bot;
using Utils::TGBotApi::Bot::InterfaceBot;
using Utils::TGBotApi::Message::Message;
using Utils::TGBotApi::Message::InterfaceMessage;
using Utils::TGBotApi::Message::Keyboard::InlineKeyboard;
using Utils::TGBotApi::Message::Keyboard::InterfaceInlineKeyboard;
using Utils::TGBotApi::Message::Keyboard::InterfaceInlineButton;
using Utils::TGBotApi::Message::Keyboard::InlineButton;
using Utils::Logger::StdOutLogger;
using Utils::Logger::get_logger;
using std::shared_ptr, std::make_shared;
using std::nullopt;

int main() {
    shared_ptr<InterfaceBot> bot = make_shared<Bot>(BOT_TOKEN);
    get_logger(new StdOutLogger());
    bot->set_webhook(WEBHOOK_URL + "/webhook");

    // 2. Запускаем сервер для обработки входящих запросов
    httplib::Server server;

    // Обработчик вебхука
    server.Post("/webhook", [&bot](const httplib::Request& req, httplib::Response& res) {
        get_logger()->info("BODY", req.body);
        try {
            json update = json::parse(req.body);
            if (update.contains("message")) {
                InterfaceMessage::ptrMessage update_message = make_shared<Message>(update["message"]);
                std::cout << "Получено: " << update_message->get_text().value_or("(none)") << std::endl;

                auto message = bot->send_message({
                    .chat_id = update_message->get_chat()->get_id(),
                    .text = update_message->get_text().value_or("(noneo)"),
                    .inline_keyboard = shared_ptr<InterfaceInlineKeyboard>(new InlineKeyboard(
                        make_shared<InlineKeyboard::Buttons>(
                            1,
                            InlineKeyboard::Lane(
                                1,
                                make_shared<InlineButton>("callback", nullopt, "hihihaha")
                            )
                        )
                    ))
                });
            }
            res.status = 200;
        } catch (const json::exception& e) {
            std::cerr << "Ошибка JSON: " << e.what() << std::endl;
            res.status = 400;
        }
    });

    server.Get(".*", [](const httplib::Request& req, httplib::Response& res) {
        res.body = "123";
        res.status = 200;
        std::cout << 123 << std::endl;
    });

    std::cout << "Сервер запущен на порту " << PORT << ". Ожидание вебхуков..." << std::endl;
    server.listen("0.0.0.0", PORT);

    return 0;
}
