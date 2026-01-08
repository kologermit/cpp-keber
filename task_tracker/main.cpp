#include <memory>
#include <csignal>
#include <pqxx/pqxx>
#include <utils/Logger/Logger.hpp>
#include <utils/HTTPServer/Server/Server.hpp>
#include <utils/Postgres/postgres.hpp>
#include <utils/BotApi/BotApi.hpp>
#include <task_tracker/Config/Config.hpp>
#include <task_tracker/GlobalContext.hpp>
#include <task_tracker/HTTPHandler/init.hpp>
#include <task_tracker/Entity/Task/TaskRepository.hpp>


using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using std::to_string;
using std::nullopt;
using pqxx::connection;
using Utils::Logger::get_logger;
using Utils::Logger::Logger;
using Utils::Logger::InterfaceLogger;
using Utils::HTTPServer::Server::InterfaceServer;
using Utils::HTTPServer::Server::Server;
using Utils::Postgres::create_connection;
using Utils::BotApi::BotApi;
using TaskTracker::Config::InterfaceConfig;
using TaskTracker::Config::Config;
using TaskTracker::GlobalContext;
using TaskTracker::DBContext;
using TaskTracker::HTTPHandler::HandlerContext;
using TaskTracker::HTTPHandler::init_server;
using TaskTracker::Entity::Task::TaskRepository;

shared_ptr<Server<GlobalContext, HandlerContext>> server;

void signal_handler(const int signal) {
    get_logger()->info("SIGNAL", to_string(signal), __FILE__, __LINE__);
    server->stop();
}

int main(int argc, const char* argv[]) {
    const shared_ptr<InterfaceConfig> config = make_shared<Config>(argc, argv);
    const shared_ptr<InterfaceLogger> logger = get_logger(make_unique<Logger>(config->get_logs_path()));

    if (config->is_help()) {
        logger->info("HELP", "THIS IS TASK TRACKER SERVICE", __FILE__, __LINE__);
        return 0;
    }

    auto db = create_connection(config->get_db_conn_url());

    const shared_ptr<GlobalContext> global_ctx = make_shared<GlobalContext>(
        GlobalContext{
            .logger = logger,
            .config = config,
            .db = make_shared<DBContext>(DBContext{
                .task = make_shared<TaskRepository>(db)
            }),
            .bot_api = make_shared<BotApi>(config->get_bot_url(), config->get_auth_key()),
        }
    );

    server = make_shared<Server<GlobalContext, HandlerContext>>(
        global_ctx,
        config->get_listen_ip(),
        static_cast<int>(config->get_listen_port())
    );

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    init_server(*server);

    global_ctx->bot_api->post_message(
        nullopt,
        "START TASK TRACKER"
    );
    server->run();

    return 0;
}
