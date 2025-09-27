#include <bot/Server/Server.hpp>
#include <bot/Signal/Signal.hpp>
#include <utils/Config/Config.hpp>
#include <utils/Logger/Logger.hpp>
#include <utils/YouTubeApi/YouTubeApi.hpp>
#include <csignal>

using Utils::Logger::get_logger;
using Utils::Logger::Logger;
using TGBot = Utils::TGBotApi::Bot::Bot;
using Utils::TGBotApi::Bot::get_bot;
using Utils::Config::get_config;
using Utils::Config::Config;
using Utils::YouTubeApi::YouTubeApi;
using Utils::YouTubeApi::get_youtube_api;
using Bot::Entity::Repositories::Repositories;
using Bot::Entity::Repositories::get_repositories;
using Bot::Server::Server;
using Bot::Server::get_server;
using Bot::Signal::signal_handler;
using std::make_unique;
using std::signal;

int main(int argc, const char* argv[]) {
    get_config(make_unique<Config>(argc, argv));
    get_logger(make_unique<Logger>(get_config()->get_logs_path()));

    if (get_config()->is_help()) {
        get_logger()->info("HELP", "THIS IS BOT SERVICE", __FILE__, __LINE__);
        return 0;
    }

    get_youtube_api(make_unique<YouTubeApi>());
    get_bot(make_unique<TGBot>(get_config()->get_bot_token()));
    get_repositories(make_unique<Repositories>(get_config()->get_db_conn_url()));
    get_server(make_unique<Server>());
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    get_bot()->set_webhook(get_config()->get_webhook_url());
    get_server()->run();

    return 0;
}
