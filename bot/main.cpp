#include <utils/Logger/StdOutLogger.hpp>
#include <utils/Env/Env.hpp>
#include <utils/TGBotApi/Bot/Bot.hpp>
#include <utils/TGBotApi/Message/Keyboard/ReplyKeyboard.hpp>
#include <utils/TGBotApi/Message/Keyboard/ReplyButton.hpp>
#include <utils/TGBotApi/Message/Keyboard/InlineKeyboard.hpp>
#include <utils/TGBotApi/Message/Keyboard/InlineButton.hpp>

using Utils::TGBotApi::File::EnumContentType;
using Utils::Logger::get_logger;
using Utils::Logger::StdOutLogger;
using Utils::TGBotApi::Bot::Bot;
using Utils::TGBotApi::Message::Keyboard::ReplyKeyboard;
using Utils::TGBotApi::Message::Keyboard::ReplyButton;
using Utils::TGBotApi::Message::Keyboard::InlineKeyboard;
using Utils::TGBotApi::Message::Keyboard::InlineButton;
using Utils::TGBotApi::Bot::ptrBot;
using std::to_string;
using std::make_shared;
using std::string;
using std::nullopt;
const int MESSAGE_ID = 2723;
const int USER_ID = 847721936;
string TOKEN = Utils::Env::Get("BOT_TOKEN");
string ANSWER_TEXT = "<code>answer</code>";
string PHOTO_FILEPATH = "/home/kologermit/audio.mp3";

int main() {
    auto logger = get_logger(new StdOutLogger);
    ptrBot bot = make_shared<Bot>(TOKEN);

    bot->send_message({
        .chat_id = USER_ID,
        .text = ANSWER_TEXT,
        .reply_message_id = MESSAGE_ID,
        .filepath = PHOTO_FILEPATH,
        .content_type = EnumContentType::AUDIO,
        .inline_keyboard = make_shared<InlineKeyboard>(
            InlineKeyboard::ptrButtons(new InlineKeyboard::Buttons{
                { make_shared<InlineButton>("Проверка callback", nullopt, "123") },
                { make_shared<InlineButton>("Ссылка на VK", "https://vk.com/kologermit"), make_shared<InlineButton>("Ссылка на ТГ", "https://t.me/kologermit.ru") }
            })
        )
    });

    auto message = bot->send_message({
        .chat_id = USER_ID,
        .text = ANSWER_TEXT,
        .reply_message_id = MESSAGE_ID,
        .filepath = PHOTO_FILEPATH,
        .content_type = EnumContentType::AUDIO,
        .reply_keyboard = make_shared<ReplyKeyboard>(
            ReplyKeyboard::ptrButtons(new ReplyKeyboard::Buttons{
                { make_shared<ReplyButton>("Проверка callback") },
                { make_shared<ReplyButton>("Ссылка на VK"), make_shared<ReplyButton>("Ссылка на ТГ") }
            })
        )
    });

    logger->info("MESSAGE_TEXT", *message->get_text());
    logger->info("MESSAGE_DOWNLOAD_ID", *message->get_file_download_id());
    logger->info("MESSAGE_CONTENT_TYPE", to_string(message->get_file_content_type()));
    logger->info("MESSAGE_DOWNLOAD_ID", *message->get_file_name());
    logger->info("MESSAGE_FILE_SIZE (kb)", to_string(*message->get_file_size() / 1024));

    logger->info("REPLY_MESSAGE_ID", to_string(message->get_reply_message()->get_id()));
    logger->info("REPLY_MESSAGE_TEXT", *message->get_reply_message()->get_text());
}