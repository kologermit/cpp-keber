#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <bot/Config/Config.hpp>
#include <bot/Entity/User/UserRepository.hpp>
#include <utils/Entity/ApiRequest/ApiRequestRepository.hpp>
#include <utils/Logger/StdOutLogger.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <nlohmann/json.hpp>
#include <httplib.h>
#include <memory>
#include <fmt/core.h>
#include <pqxx/pqxx>

using Bot::Config::get_config;
using Bot::Config::Config;
using Bot::Entity::User::InterfaceUserRepository;
using Bot::Entity::User::UserRepository;
using Bot::Entity::User::User;
using Bot::Entity::User::EnumUserScreen;
using Utils::Logger::get_logger;
using Utils::Logger::StdOutLogger;
using Utils::TGBotApi::Bot::SendMessageParameters;
using Utils::TGBotApi::Types::ptrBot;
using Utils::TGBotApi::Types::Message;
using Utils::Entity::ApiRequest::InterfaceApiRequestRepository;
using Utils::Entity::ApiRequest::ApiRequestRepository;
using Utils::Entity::ApiRequest::ApiRequest;
using Utils::Entity::ApiRequest::ApiRequestColumns;
using Utils::Entity::ApiRequest::EnumRequestService;
using std::nullopt;
using Utils::Entity::ApiRequest::EnumRequestService;
using std::shared_ptr;
using std::string;
using std::make_shared;
using std::make_unique;
using std::unique_ptr;
using std::to_string;
using std::move;
using nlohmann::json;
using fmt::format;

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

    httplib::Server server;

    server.Post(config->get_webhook_path(), [&user_repository, &bot, &api_request_repository](const httplib::Request& req, httplib::Response& res) {
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
            Message msg(update["message"]);

            auto user = user_repository->get_by_telegram_user(*msg.from);

            auto answer_message = bot->send_message(SendMessageParameters{
                .chat_id = user->telegram_id,
                .text = user->name + " " + user->username,
            });

            api_request.response = json{
                {"code", 200}
            };
            res.status = 200;
        }
        api_request_repository->create(api_request);
        logger->info("WEBHOOK REQUEST", update.dump());
    });

    bot->set_webhook(config->get_webhook_url() + config->get_webhook_path());

    bot->send_message(SendMessageParameters{
        .chat_id = config->get_admins()[0],
        .text = "START BOT"
    });

    logger->info("START BOT", format(
        "Listen {}:{}",
        config->get_listen_ip(),
        config->get_listen_port()
    ));
    server.listen(config->get_listen_ip(), config->get_listen_port());

    return 0;
}
