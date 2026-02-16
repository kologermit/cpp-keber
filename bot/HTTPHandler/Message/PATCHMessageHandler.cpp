#include <bot/HTTPHandler/Message/PATCHMessageHandler.hpp>

namespace Bot::HTTPHandler::Message {

    using std::vector;
    using std::invalid_argument;
    using Utils::TGBotApi::JSONKeys::CHAT_TELEGRAM_ID_KEY;
    using Utils::TGBotApi::JSONKeys::MESSAGE_ID_KEY;
    using Utils::TGBotApi::JSONKeys::TEXT_KEY;
    using Utils::TGBotApi::JSONKeys::CAPTION_KEY;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;
    using Utils::HTTPServer::Handler::Param;
    using Utils::HTTPServer::Handler::ParamType;

    const HandlerSignature& PATCHMessageHandler::get_signature() const noexcept {
        static const HandlerSignature signature{
            .name = "PATCHMessageHandler",
            .pattern = "/message",
            .method = RequestHandlerMethod::PATCH,
            .is_auth = true,
            .is_json_body = true,
            .body_params = vector<Param>{
                Param{CHAT_TELEGRAM_ID_KEY, ParamType::INT},
                Param{MESSAGE_ID_KEY, ParamType::INT},
                Param{TEXT_KEY, ParamType::STRING, false},
                Param{CAPTION_KEY, ParamType::STRING, false},
            },
        };
        return signature;
    }

    json PATCHMessageHandler::handle(ptrContext ctx) {
        json json_body = ctx->json_body.value();
        if (json_body.contains(TEXT_KEY)) {
            return ctx->global_ctx->bot->edit_text(
                json_body[CHAT_TELEGRAM_ID_KEY].get<long long>(),
                json_body[MESSAGE_ID_KEY].get<long long>(),
                json_body[TEXT_KEY].get<string>()
            )->id;
        } else if (json_body.contains(CAPTION_KEY)) {
            return ctx->global_ctx->bot->edit_caption(
                json_body[CHAT_TELEGRAM_ID_KEY].get<long long>(),
                json_body[MESSAGE_ID_KEY].get<long long>(),
                json_body[CAPTION_KEY].get<string>()
            )->id;
        }
        throw invalid_argument("param text or caption not found");
    }
}