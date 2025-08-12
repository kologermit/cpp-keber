#include <bot/Server/Server.hpp>
#include <bot/Handler/InterfaceBotHandler.hpp>
#include <bot/Config/InterfaceConfig.hpp>
#include <utils/Logger/InterfaceLogger.hpp>

namespace Bot::Server {

using std::move;
using std::make_shared;
using std::make_unique;
using std::to_string;
using std::exception;
using nlohmann::json;
using httplib::Request;
using httplib::Response;
using fmt::format;
using nlohmann::json;
using Utils::Logger::get_logger;
using Utils::Entity::ApiRequest::NGINX;
using Utils::Entity::ApiRequest::BOT;
using Utils::Entity::ApiRequest::ApiRequest;
using Utils::TGBotApi::Bot::SECRET_HEADER;
using Utils::TGBotApi::JSONKeys::MESSAGE_KEY;
using Utils::TGBotApi::Types::ptrMessage;
using TGMessage = Utils::TGBotApi::Message::Message;
using Bot::Handler::BotHandlerContext;
using Bot::Config::get_config;
using Bot::Entity::Message::Message;
using Bot::Entity::Chat::Chat;
using Bot::Entity::User::User;
using Bot::Entity::User::map_user_screen_to_string;

Server::Server(shared_ptr<Repositories> repositories, shared_ptr<InterfaceBot> bot):
_repositories(repositories),
_bot(bot),
_server() {}

void Server::add_bot_handler(unique_ptr<InterfaceBotHandler> handler) noexcept {
    _bot_handlers.push_back(move(handler));
}

void Server::add_request_handler(unique_ptr<InterfaceRequestHandler> handler) noexcept {
    _request_handlers.push_back(move(handler));
}

void Server::run() noexcept {
    auto& repositories = _repositories;
    auto& bot = _bot;
    auto& bot_handlers = _bot_handlers;

    _server.Post(get_config()->get_webhook_path(), [
        &repositories,
        &bot,
        &bot_handlers
    ](const Request& req, Response& res) {
        ApiRequest api_request(
            NGINX, 
            BOT,
            {
                {"method", "post"},
                {"path", req.path},
            },
            {
                {"body", ""}
            }
        );
        res.body = "";
        auto find_key = req.headers.find(SECRET_HEADER);
        json json_body;

        if (
            find_key == req.headers.end() 
            || !bot->check_secret_token(find_key->second)
            || !json::accept(req.body)
            || !(json_body = json::parse(req.body)).contains(MESSAGE_KEY)
        ){
            api_request.request["body"] = req.body;
            api_request.response["status"] = 400;
            repositories->api_request_repository->create(api_request);
            res.status = 400;
            return;
        }
        api_request.request["body"] = json_body;
        api_request.response["status"] = 200;
        res.status = 200;

        TGMessage tg_message(json_body[MESSAGE_KEY]);
        auto user = repositories->user_repository->get_by_telegram_user(*tg_message.from);
        auto chat = repositories->chat_repository->get_by_telegram_chat(*tg_message.chat);
        auto message = repositories->message_repository->get_by_telegram_message(tg_message, user->id, chat->id);

        get_logger()->info("SERVER::MESSAGE", json{
            {"user", {
                {"name", user->name},
                {"id", user->id},
                {"telegram_id", user->telegram_id},
                {"screen", map_user_screen_to_string.at(user->screen)},
            }},
            {"message", {
                {"text", message->text},
                {"id", message->id},
                {"telegram_id", message->telegram_id},
                {"content_type", to_string(message->file_content_type)},
            }}
        }.dump(), __FILE__, __LINE__);

        shared_ptr<BotHandlerContext> context(new BotHandlerContext{
            .repositories = repositories,
            .bot = bot,
            .message = *message,
            .chat = *chat,
            .user = *user,
        });

        ptrMessage result_message;

        try {
            for (auto& handler : bot_handlers) {
                if (handler->check(context)) {
                    result_message = handler->handle(context);
                    break;
                }
            }
        } catch (const exception& ex) {
            get_logger()->error("SERVER::HANDLE", ex.what(), __FILE__, __LINE__);
            api_request.response = 500;
            res.status = 500;
        }

        repositories->api_request_repository->create(api_request);
        if (result_message != nullptr) {
            repositories->message_repository->get_by_telegram_message(*result_message, user->id, chat->id);
        }

    });
    get_logger()->info("SERVER::RUN", format(
        "SERVER RUN ON {}:{}",
        get_config()->get_listen_ip(),
        get_config()->get_listen_port()
    ), __FILE__, __LINE__);
    _server.listen(get_config()->get_listen_ip(), get_config()->get_listen_port());
}

}