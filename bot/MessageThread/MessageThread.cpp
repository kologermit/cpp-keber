#include <bot/MessageThread/MessageThread.hpp>
#include <bot/BotHandler/InterfaceBotHandler.hpp>
#include <bot/BotHandler/list_handlers.hpp>
#include <fmt/format.h>
#include <chrono>

namespace Bot::MessageThread {
    using std::stop_token;
    using std::unique_lock;
    using std::make_shared;
    using std::ranges::find;
    using std::exception;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using nlohmann::json;
    using Utils::TGBotApi::Types::ptrMessage;
    using Bot::Entity::User::User;
    using Bot::Entity::Chat::Chat;
    using Bot::Entity::Message::Message;
    using Bot::Entity::Callback::Callback;
    using Bot::Entity::Access::Access;
    using Bot::Entity::Access::AccessType;
    using Bot::BotHandler::BotHandlerContext;
    using Bot::BotHandler::get_list_bot_handlers;

    void MessageThread::run() {
        _thread = jthread(_worker);
    }

    void MessageThread::stop() {
        global_context->logger->info("MESSAGE_THREAD", "Stopping message thread", __FILE__, __LINE__);
        global_context->message_queue_cv->notify_all();
        _thread.request_stop();
    }

    void MessageThread::_worker(stop_token st) {
        const auto logger = global_context->logger;
        const auto db = global_context->db;
        const auto bot = global_context->bot;
        const auto config = global_context->config;
        auto message_queue = global_context->message_queue;
        auto message_queue_mutex = global_context->message_queue_mutex;
        auto message_queue_cv = global_context->message_queue_cv;


        while (!st.stop_requested()) {
            unique_ptr<WorkerTask> task;
            if (message_queue->empty()) {
                unique_lock lock(*message_queue_mutex);
                message_queue_cv->wait(lock, [&message_queue, &st]{return st.stop_requested() || !message_queue->empty();});
                if (st.stop_requested()) {
                    break;
                }
            }
            const auto start = high_resolution_clock::now();
            task = std::move(message_queue->front());
            message_queue->pop();

            auto tg_message = make_shared<TGMessage>(
                task->tg_callback_query.has_value()
                ? std::move(*task->tg_callback_query.value().message)
                : std::move(task->tg_message.value())
            );

            shared_ptr<User> user = db->user->get_by_telegram_user(*tg_message->from);
            shared_ptr<User> bot_user = db->user->get_by_telegram_user(bot->get_user());
            shared_ptr<Chat> chat = db->chat->get_by_telegram_chat(*tg_message->chat);
            shared_ptr<Message> message = db->message->get_by_telegram_message(*tg_message);
            shared_ptr<Callback> callback = (
                task->tg_callback_query.has_value()
                ? db->callback->get_by_telegram_callback(task->tg_callback_query.value())
                : nullptr
            );

            auto access = db->access->get_by_user_id(user->id);

            if (!access.full && find(config->get_bot_admins(), user->id) != config->get_bot_admins().end()) {
                Access admin_access;
                admin_access.type = AccessType::FULL;
                admin_access.user_id = user->id;
                db->access->create(admin_access);
                access = db->access->get_by_user_id(user->id);
            }

            auto bot_handler_ctx = make_shared<BotHandlerContext>(BotHandlerContext{
                .message = message,
                .callback = callback,
                .chat = chat,
                .user = user,
                .bot_user = bot_user,
                .global_ctx = global_context,
                .db = db,
                .access = access,
                .config = config,
                .bot = bot,
            });

            ptrMessage result;

            try {
                for (const auto& handler : get_list_bot_handlers()) {
                    if (!handler->check(bot_handler_ctx)) {
                        continue;
                    }
                    ptrMessage result_tg_message = handler->handle(bot_handler_ctx);
                    json json_result = nullptr;
                    if (result_tg_message != nullptr) {
                        auto new_message = db->message->get_by_telegram_message(*result_tg_message);
                        json_result = new_message->id;
                    }
                    const auto end = high_resolution_clock::now();
                    const long long duration = duration_cast<milliseconds>(end - start).count();
                    logger->info("BOT_HANDLER::HANDLE", fmt::format(
                        "{}|{}|{}ms|{} -- {}",
                        handler->get_name(),
                        task->id,
                        duration,
                        user->name,
                        (result_tg_message != nullptr ? result_tg_message->text : "No message")
                    ), __FILE__, __LINE__);
                    break;
                }
            } catch (const exception& err) {
                logger->error("BOT_HANDLER::EXCEPTION", fmt::format(
                    "({}): {}", 
                    task->id    ,
                    err.what()
                ), __FILE__, __LINE__);
                result = bot->send_message({
                    .chat_id = chat->id,
                    .text = fmt::format(
                        "<b>Произошла ошибка: </b><i>{}</i>",
                        err.what()
                    ),
                    .reply_message_id = message->id,
                });
            }
        }
    }
}