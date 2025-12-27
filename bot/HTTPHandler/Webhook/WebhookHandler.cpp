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

    json WebhookHandler::handle(ptrContext context) {
        auto find_secret = context->request.headers.find(SECRET_HEADER);

        if (find_secret == context->request.headers.end() 
            || !context->global_context->bot->check_secret_token(find_secret->second)
            || (
                !json::parse(context->request.body).contains(MESSAGE_KEY)
                && !json::parse(context->request.body).contains(CALLBACK_QUERY_KEY)
            )
        ) {
            context->response.status = 401;
            return "not unauthorized";
        }

        int handle_id = rand_int(1, 1000000);

        auto tg_callback = make_shared<optional<TGCallbackQuery>>(
            context->json_body.value().contains(CALLBACK_QUERY_KEY)
            ? optional(TGCallbackQuery(context->json_body.value()[CALLBACK_QUERY_KEY]))
            : nullopt
        );

        auto tg_message = make_shared<TGMessage>(
            tg_callback->has_value()
            ? context->json_body.value()[CALLBACK_QUERY_KEY][MESSAGE_KEY]
            : context->json_body.value()[MESSAGE_KEY]
        );

        shared_ptr<User> user = context->global_context->user_repository->get_by_telegram_user(*tg_message->from);
        shared_ptr<Chat> chat = context->global_context->chat_repository->get_by_telegram_chat(*tg_message->chat);
        shared_ptr<Message>message = context->global_context->message_repository->get_by_telegram_message(*tg_message, true);
        shared_ptr<Callback> callback = (
            tg_callback != nullptr && *tg_callback != nullopt
            ? context->global_context->callback_repository->get_by_telegram_callback(tg_callback->value())
            : nullptr
        );

        auto access = context->global_context->access_repository->get_by_user_id(user->id);

        if (!access.full && find(context->config->get_admins(), user->id) != context->config->get_admins().end()) {
            Access admin_access;
            admin_access.type = EnumAccessType::FULL;
            admin_access.user_id = user->id;
            context->global_context->access_repository->create(admin_access);
            access = context->global_context->access_repository->get_by_user_id(user->id);
        }

        auto bot_handler_context = make_shared<BotHandlerContext>(BotHandlerContext{
            .message = message,
            .callback = callback,
            .chat = chat,
            .user = user,
            .global_context = context->global_context,
            .handler_context = context,
            .access = access,
            .config = context->global_context->config,
            .bot = context->global_context->bot,
        });

        ptrMessage result_tg_message;
        string_view handler_name;

        try {
            for (const auto& handler : get_list_bot_handlers()) {
                if (handler->check(bot_handler_context)) {
                    handler_name = handler->get_name();
                    result_tg_message = handler->handle(bot_handler_context);
                    return result_tg_message->id;
                }
            }
        } catch (const exception& err) {
            context->logger->error("BOT_HANDLER::EXCEPTION", fmt::format(
                "({}): {}", 
                handle_id,
                err.what()
            ), __FILE__, __LINE__);
            return context->global_context->bot->send_message({
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