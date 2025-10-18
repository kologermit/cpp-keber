#include <bot/HTTPHandler/EditMessageTextHandler/EditMessageTextHandler.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <utils/HTTP/HTTP.hpp>

namespace Bot::HTTPHandler::EditMessageTextHandler {

    using Bot::Entity::Repositories::get_repositories;
    using Utils::TGBotApi::Bot::get_bot;
    using Utils::TGBotApi::File::EnumContentType;
    using Utils::HTTP::validate_params;
    using Utils::HTTP::CheckType;
    using std::exception;
    using std::stoll;

    const string& EditMessageTextHandler::get_name() const noexcept {
        static const string name = "EditMessageTextHandler";
        return name;
    }

    RequestHandlerMethod EditMessageTextHandler::get_method() const noexcept {
        return PATCH;
    }

    const string& EditMessageTextHandler::get_pattern() const noexcept {
        static const string pattern = "/edit_message_text";
        return pattern;
    }

    json EditMessageTextHandler::handle(const Request& request, Response& response) {
        validate_params(request, {
            {"chat_id", CheckType::LONG_LONG, true},
            {"message_id", CheckType::LONG_LONG, true},
            {"text", CheckType::STRING, true},
        });
        const auto message = get_repositories()->message->get_by_chat_and_id(
            stoll(request.get_param_value("chat_id")),
            stoll(request.get_param_value("message_id"))
        );
        if (message == nullptr) {
            response.status = 404;
            return "message not found";
        }
        try {
            if (message->file_content_type == EnumContentType::TEXT) {
                get_bot()->edit_text(
                    message->chat_id,
                    message->id,
                    request.get_param_value("text")
                );
            } else {
                get_bot()->edit_caption(
                    message->chat_id,
                    message->id,
                    request.get_param_value("text")
                );
            }
        } catch (const exception&) {
            response.status = 400;
            return "failed to edit message";
        }   
        return true;
    }
}