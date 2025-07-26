#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <utils/TGBotApi/Types.hpp>
#include <utils/Logger/StdOutLogger.hpp>
#include <utils/Env/Env.hpp>
#include <fmt/core.h>
#include <httplib.h>
#include <string>
#include <thread>

using json = nlohmann::json;
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
using Utils::TGBotApi::Bot::SECRET_HEADER;
using Utils::TGBotApi::JSONKeys::MESSAGE_KEY;
using Utils::TGBotApi::JSONKeys::CALLBACK_QUERY_KEY;
using Utils::Types::const_c_string;
using Utils::Logger::get_logger;
using std::make_unique;
using std::nullopt, std::to_string, std::string;
using fmt::format;

const_c_string BOT_TOKEN = Utils::Env::Get("BOT_TOKEN").data();
const int ADMIN_ID = json::parse(Utils::Env::Get("BOT_ADMINS"))[0];
const_c_string WEBHOOK_URL = "https://kologermit.ru";
const_c_string WEBHOOK_PATH = "/webhook";
const int PORT = 8083;

int main() {
    const auto logger = Utils::Logger::get_logger(new Utils::Logger::StdOutLogger);
    ptrBot bot = make_unique<Bot>(BOT_TOKEN);
    bot->delete_webhook();
    bot->set_webhook(string(WEBHOOK_URL) + WEBHOOK_PATH);
    bot->send_message({
        .chat_id = ADMIN_ID,
        .text = "START BOT",
    });

    logger->info("START_SERVER", format(
        "HOST={} PORT={}",
        "0.0.0.0", PORT
    ));

    httplib::Server().Post(WEBHOOK_PATH, [&bot, &logger](const httplib::Request& req, httplib::Response& res) {
        logger->info("REQUEST_BODY", req.body);
        auto secret_iterator = req.headers.find(SECRET_HEADER);
        for (const auto& header : req.headers) {
            logger->info("HEADER", header.first + ": " + header.second);
        }
        if (secret_iterator == req.headers.end()) {
            logger->error("CHECK_HEADER_SECRET", "SECRET NOT FOUND");
            return;
        }
        logger->debug("HEADER_SECRET", secret_iterator->second);
        if (!bot->check_secret_token(secret_iterator->second)) {
            logger->error("CHECK_HEADER_SECRET", "NOT VALID SECRET");
            return;
        }
        json update = json::parse(req.body);
        if (update.contains(MESSAGE_KEY)) {
            ptrMessage update_message = make_unique<Message>(update[MESSAGE_KEY]);
            logger->info("UPDATE_TEXT", update_message->get_text());

            auto message = bot->send_message({
                .chat_id = update_message->get_chat()->get_id(),
                .text = update_message->get_text(),
                .inline_keyboard = make_unique<InlineKeyboard>({

                })
            });
        } else if (update.contains(CALLBACK_QUERY_KEY)) {j
            ptrCallbackQuery callback_query = make_unique<CallbackQuery>(update[CALLBACK_QUERY_KEY]);
            logger->info("CALLBACK", callback_query->get_data());
            logger->info("CALLBACK_ID", callback_query->get_id());
            logger->info("CALLBACK_ANSWER_RESULT", to_string(bot->answer_callback_query(callback_query->get_id())));
        }
        res.status = 200;
    }).Get(".*", [&logger](const httplib::Request& req, httplib::Response& res) {
        res.body = "404";
        res.status = 404;
        logger->info("404", "404");
    }).listen("0.0.0.0", PORT);

    return 0;
}
