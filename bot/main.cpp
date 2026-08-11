#include <memory>
#include <csignal>
#include <pqxx/pqxx>
#include <iostream>
#include <utils/Random/Random.hpp>
#include <utils/Logger/Logger.hpp>
#include <utils/TGBotApi/Bot/Bot.hpp>
#include <utils/YouTubeApi/YouTubeApi.hpp>
#include <utils/TaskTrackerApi/TaskTrackerApi.hpp>
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
#include <bot/MessageThread/MessageThread.hpp>

std::shared_ptr<
    Utils::HTTPServer::Server::InterfaceServer<
        Bot::GlobalContext, Bot::HTTPHandler::HandlerContext> > 
    server;

std::shared_ptr<Bot::MessageThread::InterfaceMessageThread> message_queue_thread;

void graceful_shutdown_by_signal(const int signal) {
    using std::to_string;
    global_context->logger->info("SIGNAL", to_string(signal), __FILE__, __LINE__);
    message_queue_thread->stop();
    server->stop();
}

std::shared_ptr<Bot::GlobalContext> global_context;

int main(int argc, const char* argv[]) {
    using std::map;
    using std::shared_ptr;
    using std::unique_ptr;
    using std::make_shared;
    using std::make_unique;
    using std::to_string;
    using std::cout;
    using std::endl;
    using std::jthread;
    using std::queue;
    using std::pair;
    using std::mutex;
    using std::condition_variable;
    using pqxx::connection;
    using Utils::Random::init_random;
    using Utils::Logger::get_logger;
    using Utils::Logger::Logger;
    using Utils::Logger::InterfaceLogger;
    using TGBot = Utils::TGBotApi::Bot::Bot;
    using Utils::TGBotApi::Types::ptrMessage;
    using Utils::YouTubeApi::YouTubeApi;
    using Utils::TaskTrackerApi::TaskTrackerApi;
    using Utils::HTTPServer::Server::Server;
    using Utils::Postgres::create_connection;
    using Bot::Config::InterfaceConfig;
    using Bot::Config::Config;
    using Bot::MinimalTask;
    using Bot::GlobalContext;
    using Bot::WorkerTask;
    using Bot::DBContext;
    using Bot::ApiContext;
    using Bot::HTTPHandler::HandlerContext;
    using Bot::HTTPHandler::init_server;
    using Bot::Entity::Access::AccessRepository;
    using Bot::Entity::Callback::CallbackRepository;
    using Bot::Entity::Chat::ChatRepository;
    using Bot::Entity::Message::MessageRepository;
    using Bot::Entity::User::UserRepository;
    using Bot::Entity::YouTubeAudioSetting::YouTubeAudioSettingRepository;
    using Bot::MessageThread::MessageThread;

    init_random();

    const shared_ptr<InterfaceConfig> config = make_shared<Config>(argc, argv);
    if (config->is_help()) {
        cout << config->get_help() << endl;
        return 0;
    }
    config->throw_if_has_exception();

    auto db = create_connection(config->get_db_conn_url());
    const auto logger = get_logger(make_unique<Logger>(config->get_logs_path()));
    const auto bot =  make_shared<TGBot>(
        config->get_bot_token(), 
        config->get_telegram_api_url(),
        config->get_proxy_host(),
        config->get_proxy_port()
    );

    global_context = make_shared<GlobalContext>(
        GlobalContext{
            .task_tracker_cache = make_shared<map<long long, MinimalTask> >(),
            .message_queue = make_shared<queue<unique_ptr<WorkerTask> > >(),
            .message_queue_mutex = make_shared<mutex>(),
            .message_queue_cv = make_shared<condition_variable>(),
            .logger = logger,
            .config = config,
            .bot = bot,
            .db = make_shared<DBContext>(DBContext{
                .access = make_shared<AccessRepository>(db),
                .callback = make_shared<CallbackRepository>(db),
                .chat = make_shared<ChatRepository>(db),
                .message = make_shared<MessageRepository>(db),
                .user = make_shared<UserRepository>(db),
                .youtube_audio_setting = make_shared<YouTubeAudioSettingRepository>(db),
            }),
            .api = make_shared<ApiContext>(ApiContext{
                .task_tracker = make_shared<TaskTrackerApi>(config->get_task_tracker_url(), config->get_auth_key()),
                .youtube = make_shared<YouTubeApi>(config->get_youtube_api_url()),
            }),
        }
    );

    server = make_shared<Server<GlobalContext, HandlerContext> >(
        global_context,
        config->get_listen_ip(),
        static_cast<int>(config->get_listen_port())
    );
    message_queue_thread = make_shared<MessageThread>();

    signal(SIGINT, graceful_shutdown_by_signal);
    signal(SIGTERM, graceful_shutdown_by_signal);
    
    init_server(*server);
    if (!bot->set_webhook(config->get_webhook_url())) {
        logger->error("WEBHOOK", "Failed to set webhook", __FILE__, __LINE__ );
        return 1;
    };
    bot->send_message({
        .chat_id = config->get_bot_admins()[0],
        .text = "START BOT"
    });

    message_queue_thread->run();
    server->run();

    return 0;
}
