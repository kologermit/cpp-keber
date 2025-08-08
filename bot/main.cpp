#include <bot/Config/Config.hpp>
#include <bot/Entity/User/UserRepository.hpp>
#include <bot/Entity/Message/MessageRepository.hpp>
#include <utils/Entity/ApiRequest/ApiRequestRepository.hpp>
#include <utils/Logger/StdOutLogger.hpp>
#include <utils/TGBotApi/Types.hpp>

using Bot::Config::get_config;
using Bot::Config::Config;
using Bot::Entity::User::InterfaceUserRepository;
using Bot::Entity::User::UserRepository;
using Bot::Entity::User::User;
using Bot::Entity::Message::Message;
using Bot::Entity::Message::MessageRepository;
using Bot::Entity::Message::InterfaceMessageRepository;
using Utils::Logger::get_logger;
using Utils::Logger::StdOutLogger;
using Utils::TGBotApi::Bot::SendMessageParameters;
using Utils::TGBotApi::Types::ptrBot;
using TGMessage = Utils::TGBotApi::Types::Message;
using TGUser = Utils::TGBotApi::Types::User;
using Utils::Entity::ApiRequest::InterfaceApiRequestRepository;
using Utils::Entity::ApiRequest::ApiRequestRepository;
using Utils::Entity::ApiRequest::ApiRequest;
using Utils::Entity::ApiRequest::EnumRequestService;
using std::make_unique;
using std::unique_ptr;
using nlohmann::json;

auto logger = get_logger(make_unique<StdOutLogger>());

int main(int argc, const char* argv[]) {
    auto config = get_config(make_unique<Config>(argc, argv));

    if (config->is_help()) {
        logger->info("HELP", "THIS IS BOT SERVICE");
        return 0;
    }

    ptrBot bot = make_unique<Utils::TGBotApi::Types::Bot>(config->get_bot_token());
    
    unique_ptr<InterfaceApiRequestRepository> api_request_repository = make_unique<ApiRequestRepository>(config->get_db_conn_url());
    unique_ptr<InterfaceUserRepository> user_repository = make_unique<UserRepository>(config->get_db_conn_url());
    unique_ptr<InterfaceMessageRepository> message_repository = make_unique<MessageRepository>(config->get_db_conn_url());
    
    user_repository->get_by_telegram_user(TGUser(
        bot->get_id(),
        bot->get_name(),
        bot->get_username()
    ));

    httplib::Server server;

    server.Post(config->get_webhook_path(), [
        &user_repository, 
        &bot, 
        &api_request_repository,
        &message_repository
    ](const httplib::Request& req, httplib::Response& res) {
        json update = json::parse(req.body);
        ApiRequest api_request(
            EnumRequestService::NGINX,
            EnumRequestService::BOT,
            json{
                {"method", "POST"},
                {"path", req.path},
                {"body", update},
            }
        );
        res.status = 400;
        if (update.contains("message")) 
        {
            TGMessage msg(update["message"]);

            auto user = user_repository->get_by_telegram_user(*msg.from);
            auto message = message_repository->get_by_telegram_message(msg);

            auto answer_message = bot->send_message(SendMessageParameters{
                .chat_id = user->telegram_id,
                .text = user->name + " " + user->username,
                .reply_message_id = message->telegram_id
            });

            message_repository->get_by_telegram_message(*answer_message);

            api_request.response = json{
                {"code", 200}
            };
            res.status = 200;
        }
        api_request_repository->create(api_request);
        logger->info("WEBHOOK REQUEST", update.dump());
    });

    bot->set_webhook(config->get_webhook_url() + config->get_webhook_path());

    auto start_message = bot->send_message(SendMessageParameters{
        .chat_id = config->get_admins()[0],
        .text = "START BOT"
    });

    message_repository->get_by_telegram_message(*start_message);

    logger->info("START BOT", format(
        "Listen {}:{}",
        config->get_listen_ip(),
        config->get_listen_port()
    ));
    server.listen(config->get_listen_ip(), config->get_listen_port());

    return 0;
}
