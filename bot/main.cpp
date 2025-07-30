// #define CPPHTTPLIB_OPENSSL_SUPPORT

#include <bot/src/Config/Config.hpp>
#include <utils/Logger/JSONSTDOutLogger.hpp>
#include <nlohmann/json.hpp>
#include <memory>


using Bot::Config::get_config;
using Bot::Config::Config;
using Utils::Logger::get_logger;
using Utils::Logger::JSONSTDOutLogger;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using std::to_string;
using nlohmann::json;

auto logger = get_logger(make_unique<JSONSTDOutLogger>());

int main(int argc, const char* argv[]) {
    auto config = get_config(make_unique<Config>(argc, argv));

    if (config->is_help()) {
        logger->info("HELP", "THIS IS BOT SERVICE");
        return 0;
    }

    logger->info("IS_HELP", to_string(config->is_help()));
    logger->info("BOT_TOKEN", config->get_bot_token());
    logger->info("BOT_ADMINS", json(config->get_admins()).dump());
    logger->info("WEBHOOK_URL", config->get_webhook_url());
    logger->info("WEBHOOK_PATH", config->get_webhook_path());
    logger->info("LISTEN_IP", config->get_listen_ip());
    logger->info("LISTEN_PORT", to_string(config->get_listen_port()));
    logger->info("DB_HOST", config->get_db_host());
    logger->info("DB_PORT", to_string(config->get_db_port()));
    logger->info("DB_NAME", config->get_db_name());
    logger->info("DB_USER", config->get_db_user());
    logger->info("DB_PASSWORD", config->get_db_password());
    logger->info("YT_SERVICE_URL", config->get_yt_service_url());
    logger->info("TASK_SERVICE_URL", config->get_task_service_url());
    logger->info("TMP_PATH", config->get_tmp_path());
    logger->info("LOGS_PATH", config->get_logs_path());
    logger->info("SHARED_PATH", config->get_shared_path());

    return 0;
}
