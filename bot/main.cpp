#include <bot/Server/Server.hpp>
#include <utils/Config/Config.hpp>
#include <utils/Logger/Logger.hpp>
#include <utils/Random/Random.hpp>

using Utils::Logger::get_logger;
using Utils::Logger::Logger;
using TGBot = Utils::TGBotApi::Bot::Bot;
using Utils::TGBotApi::Bot::get_bot;
using Utils::Config::get_config;
using Utils::Config::Config;
using Bot::Entity::Repositories::Repositories;
using Bot::Entity::Repositories::get_repositories;
using Bot::Server::Server;
using std::make_unique;

int main(int argc, const char* argv[]) {
    get_config(make_unique<Config>(argc, argv));
    get_logger(make_unique<Logger>(get_config()->get_logs_path()));

    if (get_config()->is_help()) {
        get_logger()->info("HELP", "THIS IS BOT SERVICE", __FILE__, __LINE__);
        return 0;
    }

    get_bot(make_unique<TGBot>(get_config()->get_bot_token()));
    get_repositories(make_unique<Repositories>(get_config()->get_db_conn_url()));

    Server server;
    get_bot()->set_webhook(get_config()->get_webhook_url());
    get_bot()->send_message({
        .chat_id = get_config()->get_admins()[0],
        .text = "START BOT",
    });
    server.run();

    return 0;
}
