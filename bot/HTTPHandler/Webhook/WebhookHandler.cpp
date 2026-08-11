#include <bot/HTTPHandler/Webhook/WebhookHandler.hpp>
#include <bot/BotHandler/InterfaceBotHandler.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/Random/Random.hpp>
#include <algorithm>

namespace Bot::HTTPHandler::Webhook {
    using std::lock_guard;
    using std::mutex;
    using std::make_unique;
    using TGCallbackQuery = Utils::TGBotApi::CallbackQuery::CallbackQuery;
    using TGMessage = Utils::TGBotApi::Message::Message;
    using Utils::TGBotApi::JSONKeys::MESSAGE_KEY;
    using Utils::TGBotApi::JSONKeys::CALLBACK_QUERY_KEY;
    using Utils::TGBotApi::Bot::SECRET_HEADER;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;
    using Utils::Random::rand_int;

    const HandlerSignature& WebhookHandler::get_signature() const noexcept {
        static const HandlerSignature signature{
            .name = "WebhookHandler",
            .pattern = "/webhook",
            .method = RequestHandlerMethod::POST,
            .is_json_body = true
        };
        return signature;
    }

    json WebhookHandler::handle(ptrContext ctx) {
        auto find_secret = ctx->request.headers.find(SECRET_HEADER);

        if (find_secret == ctx->request.headers.end() 
            || !ctx->global_ctx->bot->check_secret_token(find_secret->second)
            || (
                !json::parse(ctx->request.body).contains(MESSAGE_KEY)
                && !json::parse(ctx->request.body).contains(CALLBACK_QUERY_KEY)
            )
        ) {
            ctx->response.status = 401;
            return "not unauthorized";
        }

        lock_guard<mutex> lock(*ctx->global_ctx->message_queue_mutex);
        ctx->global_ctx->message_queue->push(make_unique<WorkerTask>(WorkerTask{
            .id = ctx->handle_id,
            .tg_message = ctx->json_body.value().contains(MESSAGE_KEY)
                ? optional(TGMessage(ctx->json_body.value()[MESSAGE_KEY]))
                : nullopt,
            .tg_callback_query = ctx->json_body.value().contains(CALLBACK_QUERY_KEY)
                ? optional(TGCallbackQuery(ctx->json_body.value()[CALLBACK_QUERY_KEY]))
                : nullopt
        }));

        global_context->message_queue_cv->notify_one();

        return "success";
    }
}