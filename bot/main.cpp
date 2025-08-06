#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <bot/src/Config/Config.hpp>
#include <utils/Entity/ApiRequest/ApiRequestFactory.hpp>
#include <utils/Logger/StdOutLogger.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <nlohmann/json.hpp>
#include <memory>
#include <fmt/core.h>
#include <pqxx/pqxx>

using Bot::Config::get_config;
using Bot::Config::Config;
using Utils::Logger::get_logger;
using Utils::Logger::StdOutLogger;
using Utils::TGBotApi::Bot::SendMessageParameters;
using Utils::Entity::ApiRequest::ApiRequestFactory;
using Utils::Entity::ApiRequest::ApiRequest;
using Utils::Entity::ApiRequest::ApiRequestColumns::FROM;
using Utils::Entity::ApiRequest::ApiRequestColumns::TO;
using std::nullopt;
using Utils::Entity::ApiRequest::EnumRequestService;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;
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

    auto bot = make_unique<Utils::TGBotApi::Types::Bot>(config->get_bot_token());

    auto message = bot->send_message(SendMessageParameters{
        .chat_id = config->get_admins()[0],
        .text = "START BOT",
    });

    logger->debug("db_user", config->get_db_user());

    ApiRequestFactory factory(format(
        "host={} port={} dbname={} user={} password={}",
        config->get_db_host(),
        config->get_db_port(),
        config->get_db_name(),
        config->get_db_user(),
        config->get_db_password()
    ));

    auto api_request = factory.create(ApiRequest(
        0,
        nullopt,
        nullopt,
        nullopt,
        EnumRequestService::NGINX,
        EnumRequestService::BOT,
        json::parse("{}"),
        json::parse("{}")
    ));

    api_request->from = EnumRequestService::BOT;
    api_request->to = EnumRequestService::UNKNOWN;

    factory.update(*api_request, {FROM, TO});

    bot->send_message(SendMessageParameters{
        .chat_id = config->get_admins()[0],
        .text = to_string(api_request->id),
    });

    return 0;
}
