// #define CPPHTTPLIB_OPENSSL_SUPPORT

#include <bot/src/Config/Config.hpp>
#include <utils/Logger/StdOutLogger.hpp>
#include <memory>

using Bot::Config::get_config;
using Bot::Config::Config;
using Utils::Logger::get_logger;
using Utils::Logger::StdOutLogger;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using std::to_string;

auto logger = get_logger(make_unique<StdOutLogger>());

int main(int argc, const char* argv[]) {
    auto config = get_config(make_unique<Config>(argc, argv));

    logger->info(__FILE__, __LINE__, "BOT_TOKEN", config->get_bot_token());
    logger->info(__FILE__, __LINE__, "DB_PORT", to_string(config->get_db_port()));

    return 0;
}
