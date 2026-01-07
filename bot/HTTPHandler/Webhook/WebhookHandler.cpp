#include <bot/HTTPHandler/Webhook/WebhookHandler.hpp>
#include <bot/BotHandler/InterfaceBotHandler.hpp>
#include <bot/BotHandler/list_handlers.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/Random/Random.hpp>
#include <algorithm>

namespace Bot::HTTPHandler::Webhook {

    using std::exception;
    using std::string_view;
    using std::ranges::find;
    using std::make_shared;
    using Bot::Entity::Access::Access;
    using Bot::Entity::Access::EnumAccessType;
    using Bot::Entity::Callback::Callback;
    using Bot::Entity::User::User;
    using Bot::Entity::Chat::Chat;
    using Bot::Entity::Message::Message;
    using Bot::BotHandler::BotHandlerContext;
    using Bot::BotHandler::get_list_bot_handlers;
    using TGCallbackQuery = Utils::TGBotApi::CallbackQuery::CallbackQuery;
    using TGMessage = Utils::TGBotApi::Message::Message;
    using Utils::TGBotApi::JSONKeys::MESSAGE_KEY;
    using Utils::TGBotApi::JSONKeys::CALLBACK_QUERY_KEY;
    using Utils::TGBotApi::Bot::SECRET_HEADER;
    using Utils::TGBotApi::Types::ptrMessage;
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

        int handle_id = rand_int(1, 1000000);

        auto tg_callback = make_shared<optional<TGCallbackQuery>>(
            ctx->json_body.value().contains(CALLBACK_QUERY_KEY)
            ? optional(TGCallbackQuery(ctx->json_body.value()[CALLBACK_QUERY_KEY]))
            : nullopt
        );

        auto tg_message = make_shared<TGMessage>(
            tg_callback->has_value()
            ? ctx->json_body.value()[CALLBACK_QUERY_KEY][MESSAGE_KEY]
            : ctx->json_body.value()[MESSAGE_KEY]
        );

        shared_ptr<User> user = ctx->db->user->get_by_telegram_user(*tg_message->from);
        shared_ptr<User> bot_user = ctx->db->user->get_by_telegram_user(ctx->global_ctx->bot->get_user());
        shared_ptr<Chat> chat = ctx->db->chat->get_by_telegram_chat(*tg_message->chat);
        shared_ptr<Message>message = ctx->db->message->get_by_telegram_message(*tg_message, true);
        shared_ptr<Callback> callback = (
            tg_callback != nullptr && *tg_callback != nullopt
            ? ctx->db->callback->get_by_telegram_callback(tg_callback->value())
            : nullptr
        );

        auto access = ctx->db->access->get_by_user_id(user->id);

        if (!access.full && find(ctx->config->get_bot_admins(), user->id) != ctx->config->get_bot_admins().end()) {
            Access admin_access;
            admin_access.type = EnumAccessType::FULL;
            admin_access.user_id = user->id;
            ctx->db->access->create(admin_access);
            access = ctx->db->access->get_by_user_id(user->id);
        }

        auto bot_handler_ctx = make_shared<BotHandlerContext>(BotHandlerContext{
            .message = message,
            .callback = callback,
            .chat = chat,
            .user = user,
            .bot_user = bot_user,
            .global_ctx = ctx->global_ctx,
            .handler_ctx = ctx,
            .db = ctx->db,
            .access = access,
            .config = ctx->global_ctx->config,
            .bot = ctx->global_ctx->bot,
        });

        try {
            for (const auto& handler : get_list_bot_handlers()) {
                if (!handler->check(bot_handler_ctx)) {
                    continue;
                }
                ptrMessage result_tg_message = handler->handle(bot_handler_ctx);
                json json_result = nullptr;
                if (result_tg_message != nullptr) {
                    auto new_message = ctx->db->message->get_by_telegram_message(*result_tg_message);
                    json_result = new_message->id;
                }
                ctx->logger->info("BOT_HANDLER::HANDLE", fmt::format(
                    "({}) -- {} -- {}",
                    handle_id,
                    user->name,
                    (result_tg_message != nullptr ? result_tg_message->text : "No message")
                ), __FILE__, __LINE__);
                return json_result;
            }
        } catch (const exception& err) {
            ctx->logger->error("BOT_HANDLER::EXCEPTION", fmt::format(
                "({}): {}", 
                handle_id,
                err.what()
            ), __FILE__, __LINE__);
            return ctx->global_ctx->bot->send_message({
                .chat_id = chat->id,
                .text = fmt::format(
                    "<b>Произошла ошибка: </b><i>{}</i>",
                    err.what()
                ),
                .reply_message_id = message->id,
            })->id;
        }
        return nullptr;
    }
}