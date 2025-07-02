#include <fmt/core.h>
#include <memory>
#include <utils/Env/Env.cpp>
#include <utils/Random/Random.cpp>
#include <utils/Type.cpp>
#include <utils/Logger/InterfaceLogger.cpp>
#include <utils/Logger/FileDailyLogger.cpp>
#include <Middlewares/LogMiddleware.cpp>

namespace Program {

using namespace Middlewares;
using namespace Utils::Type;
using namespace Utils;
using namespace crow;
using namespace std;
using namespace fmt;

class Program {
    private:
    const_string cache_host;
    const_string cache_port;
    const_string cache_password;
    const_string bot_port;
    const_string db_conn_str;
    App<LogMiddleware> app;

    void init_random() const {
        Random::init();
    }

    void init_app() {
        this->app.loglevel(LogLevel::WARNING);
        CROW_ROUTE(this->app, "/<string>")
        .methods(HTTPMethod::GET)
        ([](const_string a){
            JSON answer;
            answer["a"] = "a";
            return answer;
        });
    }

    void log_start() {
        const_string host = "0.0.0.0";
        GLOBAL_LOGGER->info(
            "START PROGRAM", 
            format(
                "host={host} port={port} url=http://{host}:{port}",
                arg("host", host),
                arg("port", this->bot_port)
            )
        );
    }
    
    void run_app() {
        this->app.port(atoi(this->bot_port.c_str())).multithreaded().run();
    }

    void init_logger() {
        GLOBAL_LOGGER = make_shared<FileDailyLogger>();
    }

    public:
    Program():
        cache_host(Utils::Env::Get("CACHE_HOST", "localhost")),
        cache_port(Utils::Env::Get("CACHE_PORT", "6379")),
        cache_password(Utils::Env::Get("CACHE_PASSWORD", "qwerty")),
        bot_port(Utils::Env::Get("BOT_PORT", "8080")),
        db_conn_str(
            fmt::format(
                "host={} port={} dbname={} user={} password={}",
                Utils::Env::Get("DB_HOST", "localhost"),
                Utils::Env::Get("DB_PORT", "5432"),
                Utils::Env::Get("DB_NAME", "bot"),
                Utils::Env::Get("DB_USER", "postgres"),
                Utils::Env::Get("DB_PASSWORD", "qwerty")
            )
        ) 
    {
        this->init_random();
        this->init_logger();
        this->init_app();
    };

    void run() {
        this->log_start();
        this->run_app();
    }
};

}