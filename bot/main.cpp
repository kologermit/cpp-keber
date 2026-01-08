#include <memory>
#include <csignal>
#include <pqxx/pqxx>
#include <utils/Random/Random.hpp>
#include <utils/Logger/Logger.hpp>
#include <utils/TGBotApi/Bot/Bot.hpp>
#include <utils/YouTubeApi/YouTubeApi.hpp>
#include <utils/HTTPServer/Server/Server.hpp>
#include <utils/Postgres/postgres.hpp>
#include <bot/Config/Config.hpp>
#include <bot/GlobalContext.hpp>
#include <bot/HTTPHandler/init.hpp>
#include <bot/Entity/Access/AccessRepository.hpp>
#include <bot/Entity/Callback/CallbackRepository.hpp>
#include <bot/Entity/Chat/ChatRepository.hpp>
#include <bot/Entity/Message/MessageRepository.hpp>
#include <bot/Entity/User/UserRepository.hpp>
#include <bot/Entity/YouTubeAudioSetting/YouTubeAudioSettingRepository.hpp>

using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using std::to_string;
using pqxx::connection;
using Utils::Random::init_random;
using Utils::Logger::get_logger;
using Utils::Logger::Logger;
using Utils::Logger::InterfaceLogger;
using TGBot = Utils::TGBotApi::Bot::Bot;
using Utils::YouTubeApi::YouTubeApi;
using Utils::HTTPServer::Server::InterfaceServer;
using Utils::HTTPServer::Server::Server;
using Utils::Postgres::create_connection;
using Bot::Config::InterfaceConfig;
using Bot::Config::Config;
using Bot::GlobalContext;
using Bot::DBContext;
using Bot::HTTPHandler::HandlerContext;
using Bot::HTTPHandler::init_server;
using Bot::Entity::Access::AccessRepository;
using Bot::Entity::Callback::CallbackRepository;
using Bot::Entity::Chat::ChatRepository;
using Bot::Entity::Message::MessageRepository;
using Bot::Entity::User::UserRepository;
using Bot::Entity::YouTubeAudioSetting::YouTubeAudioSettingRepository;

shared_ptr<Server<GlobalContext, HandlerContext>> server;

void signal_handler(const int signal) {
    get_logger()->info("SIGNAL", to_string(signal), __FILE__, __LINE__);
    server->stop();
}

int main(int argc, const char* argv[]) {
    init_random();

    const shared_ptr<InterfaceConfig> config = make_shared<Config>(argc, argv);

    const shared_ptr<InterfaceLogger> logger = get_logger(make_unique<Logger>(config->get_logs_path()));
    if (config->is_help()) {
        logger->info("HELP", "THIS IS BOT SERVICE", __FILE__, __LINE__);
        return 0;
    }

    auto db = create_connection(config->get_db_conn_url());

    const shared_ptr<GlobalContext> global_context = make_shared<GlobalContext>(
        GlobalContext{
            .logger = logger,
            .config = config,
            .bot = make_shared<TGBot>(config->get_bot_token(), config->get_telegram_api_url()),
            .db = make_shared<DBContext>(DBContext{
                .access = make_shared<AccessRepository>(db),
                .callback = make_shared<CallbackRepository>(db),
                .chat = make_shared<ChatRepository>(db),
                .message = make_shared<MessageRepository>(db),
                .user = make_shared<UserRepository>(db),
                .youtube_audio_setting = make_shared<YouTubeAudioSettingRepository>(db),
            }),
            .youtube_api = make_shared<YouTubeApi>(config->get_youtube_api_url()),
        }
    );

    server = make_shared<Server<GlobalContext, HandlerContext>>(
        global_context,
        config->get_listen_ip(),
        static_cast<int>(config->get_listen_port())
    );

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    init_server(*server);
    if (!global_context->bot->set_webhook(config->get_webhook_url())) {
        logger->error("WEBHOOK", "Failed to set webhook", __FILE__, __LINE__ );
        return 1;
    };
    global_context->bot->send_message({
        .chat_id = config->get_bot_admins()[0],
        .text = "START BOT"
    });
    server->run();

    return 0;
}
