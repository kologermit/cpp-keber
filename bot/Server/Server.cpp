#include <bot/Server/Server.hpp>
#include <bot/BotHandler/NotFound/NotFound.hpp>
#include <bot/BotHandler/Dice/Dice.hpp>
#include <bot/HTTPHandler/CheckHealth/CheckHealth.hpp>
#include <utils/Logger/InterfaceLogger.hpp>
#include <utils/Random/Random.hpp>
#include <utils/JSONKeys.hpp>
#include <utils/Config/InterfaceConfig.hpp>

namespace Bot::Server {

using std::move;
using std::make_shared;
using std::make_unique;
using std::to_string;
using std::exception;
using std::string;
using std::stoi;
using nlohmann::json;
using httplib::Request;
using httplib::Response;
using RequestHandler = httplib::Server::Handler;
using fmt::format;
using Utils::Logger::get_logger;
using Utils::Entity::ApiRequest::NGINX;
using Utils::Entity::ApiRequest::BOT;
using Utils::Entity::ApiRequest::UNKNOWN;
using Utils::Entity::ApiRequest::ApiRequest;
using Utils::Entity::ApiRequest::FROM_HEADER;
using Utils::Entity::ApiRequest::EnumRequestService;
using Utils::TGBotApi::Bot::SECRET_HEADER;
using Utils::TGBotApi::Bot::get_bot;
using Utils::TGBotApi::JSONKeys::MESSAGE_KEY;
using Utils::TGBotApi::JSONKeys::ID_KEY;
using Utils::TGBotApi::Types::ptrMessage;
using Utils::TGBotApi::Query::GET;
using Utils::TGBotApi::Query::POST;
using Utils::TGBotApi::File::convert_map_content_type;
using Utils::TGBotApi::File::TEXT;
using Utils::Random::rand_int;
using Utils::Config::get_config;
using Utils::JSONKeys::ERROR_KEY;
using Utils::JSONKeys::PATH_KEY;
using Utils::JSONKeys::STATUS_KEY;
using Utils::JSONKeys::BODY_KEY;
using Utils::JSONKeys::METHOD_KEY;
using Utils::JSONKeys::PATTERN_KEY;
using Utils::JSONKeys::RESULT_KEY;
using TGMessage = Utils::TGBotApi::Message::Message;
using Bot::HTTPHandler::CheckHealth::CheckHealth;
using Bot::HTTPHandler::RequestHandlerMethod;
using Bot::BotHandler::NotFound::NotFound;
using Bot::BotHandler::BotHandlerContext;
using Bot::BotHandler::Dice::Dice;
using Bot::Entity::Message::Message;
using Bot::Entity::Chat::Chat;
using Bot::Entity::User::User;
using Bot::Entity::User::map_user_screen_to_string;
using Bot::Entity::Repositories::get_repositories;
using Bot::Entity::Message::TELEGRAM_ID_COLUMN;

Server::Server(
    vector<unique_ptr<InterfaceBotHandler> >&& bot_handlers,
    vector<unique_ptr<InterfaceHTTPHandler> >&& request_handlers
):
bot_handlers(move(bot_handlers)),
request_handlers(move(request_handlers))
{}

Server::Server()
{
    bot_handlers.emplace_back(make_unique<Dice>());
    bot_handlers.emplace_back(make_unique<NotFound>());
    request_handlers.emplace_back(make_unique<CheckHealth>());
}

struct BotHandler : InterfaceHTTPHandler {

    const string& get_pattern() const noexcept override {
        return get_config()->get_webhook_path(); 
    }

    RequestHandlerMethod get_method() const noexcept override {
        return RequestHandlerMethod::POST;
    }

    BotHandler(vector<unique_ptr<InterfaceBotHandler> >& bot_handlers):
    _bot_handlers(bot_handlers) {}

    json handle(const Request& req, Response& res) override;

    private:

    vector<unique_ptr<InterfaceBotHandler> >& _bot_handlers;

};

struct NotFoundHandler : InterfaceHTTPHandler {
    const string& get_pattern() const noexcept override {
        static const string pattern = ".*";
        return pattern;
    }

    json handle(const Request&, Response& res) override {
        res.status = 404;
        return "not found";
    }
};

struct NotFoundHandlerGET : NotFoundHandler {
    RequestHandlerMethod get_method() const noexcept {
        return RequestHandlerMethod::GET;
    }
};

struct NotFoundHandlerPOST : NotFoundHandler {
    RequestHandlerMethod get_method() const noexcept {
        return RequestHandlerMethod::POST;
    }
};

struct NotFoundHandlerPATCH : NotFoundHandler {
    RequestHandlerMethod get_method() const noexcept {
        return RequestHandlerMethod::PATCH;
    }
};

struct NotFoundHandlerDELETE : NotFoundHandler {
    RequestHandlerMethod get_method() const noexcept {
        return RequestHandlerMethod::DELETE;
    }
};

void Server::run() noexcept {
    request_handlers.emplace_back(make_unique<BotHandler>(bot_handlers));
    request_handlers.emplace_back(make_unique<NotFoundHandlerGET>());
    request_handlers.emplace_back(make_unique<NotFoundHandlerPOST>());
    request_handlers.emplace_back(make_unique<NotFoundHandlerPATCH>());
    request_handlers.emplace_back(make_unique<NotFoundHandlerDELETE>());
    for (auto& request_handler : request_handlers) {
        string pattern = request_handler->get_pattern();
        RequestHandlerMethod method = request_handler->get_method();
        RequestHandler handler = [
            _request_handler = shared_ptr<InterfaceHTTPHandler>(move(request_handler))
        ](const Request& req, Response& res) {
            try {
                res.status = 200;
                auto result = _request_handler->handle(req, res);
                get_logger()->info("SERVER::REQUEST", format(
                    "({}) {} {}",
                    req.method,
                    res.status,
                    req.path
                ), __FILE__, __LINE__);
                res.set_content(json{{STATUS_KEY, res.status}, {RESULT_KEY, result}}.dump(), "application/json");
                ApiRequest api_request(
                    UNKNOWN,
                    BOT,
                    {
                        {METHOD_KEY, req.method},
                        {PATTERN_KEY, _request_handler->get_pattern()},
                        {PATH_KEY, req.path},
                        {BODY_KEY, req.body},
                    },
                    {
                        {STATUS_KEY, res.status},
                        {BODY_KEY, res.body},
                    }
                );
                if (req.headers.find(FROM_HEADER) != req.headers.end()) {
                    api_request.from = static_cast<EnumRequestService>(stoi(req.headers.find(FROM_HEADER)->second));
                }
                if (res.headers.find(FROM_HEADER) != res.headers.end()) {
                    api_request.from = static_cast<EnumRequestService>(stoi(res.headers.find(FROM_HEADER)->second));
                }
                if (json::accept(req.body)) {
                    api_request.request[BODY_KEY] = json::parse(req.body);
                }
                if (json::accept(res.body)) {
                    api_request.response[BODY_KEY] = json::parse(res.body);
                }
                get_repositories()->api_request_repository->create(api_request);
            } catch (const exception& er) {
                res.status = 500;
                get_logger()->error("SERVER::HANDLE", er.what(), __FILE__, __LINE__);
            }
        };

        if (method == RequestHandlerMethod::GET) {
            _server.Get(pattern, handler);
        } else if (method == RequestHandlerMethod::POST) {
            _server.Post(pattern, handler);
        } else if (method == RequestHandlerMethod::PATCH) {
            _server.Patch(pattern, handler);
        } else if (method == RequestHandlerMethod::DELETE) {
            _server.Delete(pattern, handler);
        }
    }
    get_logger()->info("SERVER::RUN", format(
        "SERVER RUN ON {}:{}",
        get_config()->get_listen_ip(),
        get_config()->get_listen_port()
    ), __FILE__, __LINE__);
    _server.listen(get_config()->get_listen_ip(), get_config()->get_listen_port());
}


json BotHandler::handle(const Request& req, Response& res) {
    res.headers.insert({FROM_HEADER, to_string(NGINX)});

    auto find_key = req.headers.find(SECRET_HEADER);
    json json_body;

    if (
        find_key == req.headers.end() 
        || !get_bot()->check_secret_token(find_key->second)
        || !json::accept(req.body)
        || !(json_body = json::parse(req.body)).contains(MESSAGE_KEY)
    ){
        res.status = 401;
        return {{ERROR_KEY, "unauthorized"}};
    }

    TGMessage tg_message(json_body[MESSAGE_KEY]);
    auto user = get_repositories()->user_repository->get_by_telegram_user(*tg_message.from);
    auto chat = get_repositories()->chat_repository->get_by_telegram_chat(*tg_message.chat);
    auto message = get_repositories()->message_repository->get_by_telegram_message(tg_message, user->id, chat->id);

    int handle_id = rand_int(1, 1000000);

    get_logger()->info("BOT_HANDLER::MESSAGE", format(
        "{}: {} ({}|{}|{}) -- ({}){} {}",
        handle_id,
        user->name,
        map_user_screen_to_string.at(user->screen),
        user->id,
        user->telegram_id,
        convert_map_content_type.at(message->file_content_type),
        message->file_name,
        message->text
    ), __FILE__, __LINE__);

    shared_ptr<BotHandlerContext> context(new BotHandlerContext{
        .message = *message,
        .chat = *chat,
        .user = *user,
    });

    ptrMessage result_tg_message;
    try {
        for (auto& handler : _bot_handlers) {
            if (handler->check(context)) {
                result_tg_message = handler->handle(context);
                break;
            }
        }
    } catch (const exception& err) {
        get_logger()->error("BOT_HANDLER::EXCEPTION", format(
            "({}): {}",
            handle_id,
            err.what()
        ));
        get_bot()->send_message({
            .chat_id = context->chat.telegram_id,
            .text = format(
                "<b>Произошла ошибка: </b><i>{}</i>",
                err.what()
            ),
            .reply_message_id = context->message.telegram_id,
        });
    }

    if (result_tg_message != nullptr) {
        get_logger()->info("BOT_HANDLER::MESSAGE_ANSWER", format(
            "{}: ({}){} {}",
            handle_id,
            convert_map_content_type.at(result_tg_message->file_content_type),
            result_tg_message->file_name,
            result_tg_message->text
        ));
        auto result_message = get_repositories()->message_repository->get_by_telegram_message(*result_tg_message, user->id, chat->id);
        return {{ID_KEY, result_message->id}, {TELEGRAM_ID_COLUMN, result_message->telegram_id}};
    }

    return {};

}

}