#include <bot/HTTPHandler/SendMessageHandler/SendMessageHandler.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <utils/Config/InterfaceConfig.hpp>
#include <fstream>
#include <string>

namespace Bot::HTTPHandler::SendMessageHandler {

    using Bot::Entity::Repositories::get_repositories;
    using Utils::TGBotApi::Bot::get_bot;
    using Utils::TGBotApi::Bot::SendMessageParameters;
    using Utils::TGBotApi::Message::Message;
    using Utils::TGBotApi::File::DOCUMENT;
    using Utils::Config::get_config;
    using std::stoi;
    using std::exception;
    using std::unique_ptr;
    using std::ifstream;

    const string& SendMessageHandler::get_name() const noexcept {
        static const string name = "SendMessageHandler";
        return name;    
    }

    RequestHandlerMethod SendMessageHandler::get_method() const noexcept {
        return RequestHandlerMethod::POST;
    }

    const string& SendMessageHandler::get_pattern() const noexcept {
        static const string pattern =  "/send_message";
        return pattern;
    }

    json SendMessageHandler::handle(const Request& req, Response& res) {
        res.status = 400;
        SendMessageParameters send_message_parameters;
        const auto chat_id_str = req.get_param_value("chat_id");
        if (chat_id_str.empty()) {
            return "param chat_id not found";
        }
        try {
            send_message_parameters.chat_id = stoi(chat_id_str);
        } catch ([[maybe_unused]]const exception& exc) {
            return "invalid param chat_id";
        }
        const auto chat = get_repositories()->chat->get_by_id(send_message_parameters.chat_id);
        if (chat == nullptr) {
            return "chat not found";
        }
        send_message_parameters.text = req.get_param_value("text");
        if (send_message_parameters.text.empty()) {
            return "param text not found";
        }
        const auto reply_message_id_str = req.get_param_value("reply_message_id");
        if (!reply_message_id_str.empty()) {
            try {
                send_message_parameters.reply_message_id = stoi(reply_message_id_str);
                const auto reply_message = get_repositories()->message->get_by_chat_and_id(chat->id, send_message_parameters.reply_message_id);
                if (reply_message == nullptr) {
                    return "reply message not found";
                } 
            } catch ([[maybe_unused]]const exception& ex) {
                return "invalid param reply_message_id";
            }
        }
        const auto file_path = req.get_param_value("file");
        if (!file_path.empty()) {
            send_message_parameters.filepath = fmt::format("{}/{}", get_config()->get_file_buffer_path(), file_path);
            ifstream file(send_message_parameters.filepath);
            if (!file.good()) {
                return fmt::format("cant read file {}", file_path);
            }
            send_message_parameters.content_type = DOCUMENT;
        }
        unique_ptr<Message> message;
        try {
            message = get_bot()->send_message(send_message_parameters);
        } catch (const exception& exc) {
            return fmt::format("error while send message: {}", exc.what());
        }
        res.status = 200;
        return get_repositories()->message->get_by_telegram_message(*message, false)->id;
    }
}