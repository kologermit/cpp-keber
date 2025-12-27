#include <memory>
#include <utility>
#include <csignal>
#include <pqxx/pqxx>
#include <utils/Random/Random.hpp>
#include <utils/Config/Config.hpp>
#include <utils/Logger/Logger.hpp>
#include <utils/TGBotApi/Bot/Bot.hpp>
#include <utils/YouTubeApi/YouTubeApi.hpp>
#include <utils/HTTPServer/Server/Server.hpp>
#include <bot/Signal/Signal.hpp>
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
using pqxx::connection;
using Utils::Random::init_random;
using Utils::Logger::get_logger;
using Utils::Logger::Logger;
using Utils::Logger::InterfaceLogger;
using Utils::Config::InterfaceConfig;
using Utils::Config::Config;
using TGBot = Utils::TGBotApi::Bot::Bot;
using Utils::YouTubeApi::YouTubeApi;
using Utils::HTTPServer::Server::InterfaceServer;
using Utils::HTTPServer::Server::Server;
using Bot::Signal::signal_handler;
using Bot::GlobalContext;
using Bot::HTTPHandler::HandlerContext;
using Bot::HTTPHandler::init_server;
using Bot::Entity::Access::AccessRepository;
using Bot::Entity::Callback::CallbackRepository;
using Bot::Entity::Chat::ChatRepository;
using Bot::Entity::Message::MessageRepository;
using Bot::Entity::User::UserRepository;
using Bot::Entity::YouTubeAudioSetting::YouTubeAudioSettingRepository;

int main(int argc, const char* argv[]) {
    init_random();

    shared_ptr<InterfaceConfig> config = make_shared<Config>(argc, argv);

    shared_ptr<InterfaceLogger> logger = get_logger(make_unique<Logger>(config->get_logs_path()));
    if (config->is_help()) {
        logger->info("HELP", "THIS IS BOT SERVICE", __FILE__, __LINE__);
        return 0;
    }

    connection db(config->get_db_conn_url());

    shared_ptr<GlobalContext> global_context = make_shared<GlobalContext>(
        GlobalContext{
            .logger = logger,
            .config = config,
            .bot = make_shared<TGBot>(config->get_bot_token()),
            .access_repository = make_shared<AccessRepository>(db),
            .callback_repository = make_shared<CallbackRepository>(db),
            .chat_repository = make_shared<ChatRepository>(db),
            .message_repository = make_shared<MessageRepository>(db),
            .user_repository = make_shared<UserRepository>(db),
            .youtube_audio_setting_repository = make_shared<YouTubeAudioSettingRepository>(db),
            .youtube_api = make_shared<YouTubeApi>(config->get_youtube_api_url()),
            .auth_key = "hihihaha"
        }
    );

    Server<GlobalContext, HandlerContext> server(
        global_context,
        config->get_listen_ip(),
        config->get_listen_port()
    );

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    init_server(server);
    global_context->bot->set_webhook(config->get_webhook_url());
    global_context->bot->send_message({
        .chat_id = config->get_admins()[0],
        .text = "START BOT"
    });
    server.run();

    return 0;
}
