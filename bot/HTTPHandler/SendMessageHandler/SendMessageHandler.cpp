#include <bot/HTTPHandler/SendMessageHandler/SendMessageHandler.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <utils/Config/InterfaceConfig.hpp>
#include <utils/HTTP/HTTP.hpp>
#include <fstream>
#include <string>

namespace Bot::HTTPHandler::SendMessageHandler {

    using Bot::Entity::Repositories::get_repositories;
    using Utils::TGBotApi::Bot::get_bot;
    using Utils::TGBotApi::Bot::SendMessageParameters;
    using Utils::TGBotApi::Message::Message;
    using Utils::TGBotApi::File::DOCUMENT;
    using Utils::Config::get_config;
    using Utils::HTTP::validate_params;
    using Utils::HTTP::LONG_LONG;
    using Utils::HTTP::STRING;
    using Utils::HTTP::FILE;
    using std::stoll;
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

    json SendMessageHandler::handle(const Request& request, Response& response) {
        response.status = 400;
        validate_params(request, {
            {"chat_id", LONG_LONG, true},
            {"text", STRING, true},
            {"reply_message_id", LONG_LONG, false},
            {"file", FILE, false},
        });
        SendMessageParameters send_message_parameters{
            .chat_id = stoll(request.get_param_value("chat_id")),
            .text = request.get_param_value("text")
        };
        const auto chat = get_repositories()->chat->get_by_id(send_message_parameters.chat_id);
        if (chat == nullptr) {
            return "chat not found";
        }
        const auto reply_message_id_str = request.get_param_value("reply_message_id");
        if (!reply_message_id_str.empty()) {
            send_message_parameters.reply_message_id = stoll(reply_message_id_str);
            const auto reply_message = get_repositories()->message->get_by_chat_and_id(chat->id, send_message_parameters.reply_message_id);
            if (reply_message == nullptr) {
                return "reply message not found";
            } 
        }
        const auto file_path = request.get_param_value("file");
        if (!file_path.empty()) {
            send_message_parameters.filepath = fmt::format("{}/{}", get_config()->get_file_buffer_path(), file_path);
            send_message_parameters.content_type = DOCUMENT;
        }
        unique_ptr<Message> message;
        try {
            message = get_bot()->send_message(send_message_parameters);
        } catch (const exception& exc) {
            return fmt::format("error while send message: {}", exc.what());
        }
        response.status = 200;
        return get_repositories()->message->get_by_telegram_message(*message, false)->id;
    }
}