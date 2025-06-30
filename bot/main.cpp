#include <fmt/core.h>
#include <iostream>
#include <utils/Env.cpp>
#include <utils/Random.cpp>
#include <utils/Types.cpp>
#include <Middlewares/LogMiddleware.cpp>

using Middlewares::LogMiddleware;
using Utils::Types::ConstString;
using Utils::Types::App;
using Utils::Types::LogLevel;
using Utils::Types::JSON;
using Utils::Types::HTTPMethod;

class Program {
    private:
    ConstString cache_host;
    ConstString cache_port;
    ConstString cache_password;
    ConstString bot_port;
    ConstString db_conn_str;
    App<LogMiddleware> app;

    void init_random() const {
        Utils::Random::init();
    }

    void init_app() {
        this->app.loglevel(LogLevel::WARNING);
        CROW_ROUTE(this->app, "/<string>")
        .methods(HTTPMethod::GET)
        ([](ConstString a){
            JSON answer;
            answer["a"] = "a";
            return answer;
        });
    }

    void log_start() {
        std::cout << "Start keber-bot on 0.0.0.0:" << this->bot_port << std::endl;
    }
    
    void run_app() {
        this->app.port(atoi(this->bot_port.c_str())).multithreaded().run();
    }

    public:
    Program():
        cache_host(Utils::Env::get("CACHE_HOST", "localhost")),
        cache_port(Utils::Env::get("CACHE_PORT", "6379")),
        cache_password(Utils::Env::get("CACHE_PASSWORD", "qwerty")),
        bot_port(Utils::Env::get("BOT_PORT", "8080")),
        db_conn_str(
            fmt::format(
                "host={} port={} dbname={} user={} password={}",
                Utils::Env::get("DB_HOST", "localhost"),
                Utils::Env::get("DB_PORT", "5432"),
                Utils::Env::get("DB_NAME", "bot"),
                Utils::Env::get("DB_USER", "postgres"),
                Utils::Env::get("DB_PASSWORD", "qwerty")
            )
        ) 
    {
        this->init_random();
        this->init_app();
    };

    void run() {
        this->log_start();
        this->run_app();
    }
};

int main() {
    Program().run();
}