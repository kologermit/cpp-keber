#include <bot/HTTPHandler/Message/POSTMessageHandler.hpp>
#include <filesystem>

namespace Bot::HTTPHandler::Message {

    using std::vector;
    using std::invalid_argument;
    using std::exception;
    using std::make_unique;
    using std::filesystem::exists;
    using std::filesystem::is_regular_file;
    using std::filesystem::path;
    using Utils::TGBotApi::Bot::EnumContentType;
    using Utils::TGBotApi::Bot::SendMessageParameters;
    using Utils::TGBotApi::Message::Keyboard::InlineKeyboard;
    using Utils::TGBotApi::Message::Keyboard::ReplyKeyboard;
    using Utils::TGBotApi::JSONKeys::TEXT_KEY;
    using Utils::TGBotApi::JSONKeys::CHAT_TELEGRAM_ID_KEY;
    using Utils::TGBotApi::JSONKeys::CONTENT_TYPE_KEY;
    using Utils::TGBotApi::JSONKeys::FILE_NAME_KEY;
    using Utils::TGBotApi::JSONKeys::REPLY_TO_MESSAGE_ID_KEY;
    using Utils::TGBotApi::JSONKeys::INLINE_KEYBOARD_KEY;
    using Utils::TGBotApi::JSONKeys::REPLY_MARKUP_KEY;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;
    using Utils::HTTPServer::Handler::Param;
    using Utils::HTTPServer::Handler::ParamType;

    const HandlerSignature& POSTMessageHandler::get_signature() const noexcept {
        static const HandlerSignature signature{
            .name = "POSTMessage",
            .pattern = "/message",
            .method = RequestHandlerMethod::POST,
            .is_auth = true,
            .is_json_body = true,
            .body_params = {
                Param{CHAT_TELEGRAM_ID_KEY, ParamType::INT},
                Param{TEXT_KEY, ParamType::STRING},
                Param{FILE_NAME_KEY, ParamType::STRING, false},
                Param{CONTENT_TYPE_KEY, ParamType::INT, false},
                Param{REPLY_TO_MESSAGE_ID_KEY, ParamType::INT, false},
                Param{INLINE_KEYBOARD_KEY, ParamType::ARRAY, false},
                Param{REPLY_MARKUP_KEY, ParamType::ARRAY, false},
            }
        };
        return signature;
    }

    json POSTMessageHandler::handle(ptrContext context) {
        const json& json_body = context->json_body.value();
        SendMessageParameters message_params{
            .chat_id = json_body[CHAT_TELEGRAM_ID_KEY].get<long long>(),
            .text = json_body[TEXT_KEY].get<string>(),
        };
        if (json_body.contains(REPLY_TO_MESSAGE_ID_KEY)) {
            message_params.reply_message_id = json_body[REPLY_TO_MESSAGE_ID_KEY].get<long long>();
        }
        if (json_body.contains(FILE_NAME_KEY) && json_body.contains(CONTENT_TYPE_KEY)) {
            EnumContentType content_type = static_cast<EnumContentType>(json_body[CONTENT_TYPE_KEY].get<int>());
            if (content_type <= EnumContentType::UNKNOWN || content_type > EnumContentType::TEXT) {
                throw invalid_argument(fmt::format("param {} is invalid", CONTENT_TYPE_KEY));
            }
            string file_name = json_body[FILE_NAME_KEY].get<string>();
            string file_path = path(context->config->get_file_buffer_path()) / file_name;
            #ifndef NDEBUG
            context->global_context->logger->debug("SendMessage::FILE_PATH", file_path, __FILE__, __LINE__);
            #endif
            if (!exists(file_path)) {
                throw invalid_argument(fmt::format("file {} not found", file_name));
            }
            message_params.content_type = content_type;
            message_params.filename = file_name;
            message_params.filepath = file_path;
        }
        const char* param_name;
        try {
            if (json_body.contains(INLINE_KEYBOARD_KEY)) {
                param_name = INLINE_KEYBOARD_KEY;
                message_params.inline_keyboard = make_unique<InlineKeyboard>(json_body[INLINE_KEYBOARD_KEY]);
            }
            if (json_body.contains(REPLY_MARKUP_KEY)) {
                param_name = REPLY_MARKUP_KEY;
                message_params.reply_keyboard = make_unique<ReplyKeyboard>(json_body[REPLY_MARKUP_KEY]);
            }
        } catch (const exception& exc) {
            throw invalid_argument(fmt::format(
                "param {} is invalid ({})",
                param_name,
                exc.what()
            ));
        }

        return context->global_context->bot->send_message(message_params)->id;
    }
}