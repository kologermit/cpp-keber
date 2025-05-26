#include <pqxx/pqxx>
#include <crow.h>
#include <fmt/core.h>
#include <iostream>
#include <vector>
#include <hiredis/hiredis.h>
#include <utils/env.cpp>
#include <utils/random.cpp>

typedef const char* c_string;
typedef redisContext* RedisConn;
typedef redisReply* RedisCommand;
typedef pqxx::connection DBConnection;
typedef pqxx::work DBWorker;
typedef crow::json::wvalue JSON;
using namespace std;
using namespace crow;

struct LogMiddleware {
    struct context {
        int id;
    };
    void before_handle(request& req, response& res, context& ctx) {
        ctx.id = utils::randint(1, 1000000);
        cout << fmt::format(
            "event={}; request_id={}; method={}; uri={}\n",
            "new-request",
            ctx.id,
            method_name(req.method),
            req.url
        );
    };

    void after_handle(request& req, response& res, context& ctx) {};
};

int main()
{

    utils::init_random();

    const string
        db_host = utils::get_env("DB_HOST", "localhost"),
        db_port = utils::get_env("DB_PORT", "5432"),
        db_name = utils::get_env("DB_NAME", "bot"),
        db_user = utils::get_env("DB_USER", "postgres"),
        db_password = utils::get_env("DB_PASSWORD", "qwerty"),
        cache_host = utils::get_env("CACHE_HOST", "localhost"),
        cache_port = utils::get_env("CACHE_PORT", "6379"),
        cache_password = utils::get_env("CACHE_PASSWORD", "qwerty"),
        port = utils::get_env("BOT_PORT", "8080");

    const string db_conn_str = fmt::format(
        "host={} port={} dbname={} user={} password={}",
        db_host,
        db_port,
        db_name,
        db_user,
        db_password
    );
    cout << "DB CONN: " << db_conn_str << endl;


    RedisConn cache_conn = redisConnect(cache_host.c_str(), atoi(cache_port.c_str()));
    RedisCommand reply = (RedisCommand)redisCommand(cache_conn, "PING");
    cout << "Ответ Redis: " << reply->str << std::endl;


    DBConnection connectionObject(db_conn_str);
    DBWorker worker(connectionObject);


    App<LogMiddleware> app;
    app.loglevel(LogLevel::WARNING);
    CROW_ROUTE(app, "/<string>")
    .methods(HTTPMethod::GET)
    ([&worker](const string a){
        json::wvalue answer;
        for (auto row : worker.exec("\
            SELECT\
                1 AS \"id\",\
                'Фёдор' AS \"name\",\
                'start' AS \"screen\",\
                '2025-01-01 00:00:00' AS \"created\"\
        ")) {
            for (auto cell : row) {
                vector<string> a = {string(cell.c_str())};
                answer[cell.name()] = a;
            }
        }
        answer["a"] = a;
        return answer;
    });

    cout << "Start cpp-keber" << endl;
    app.port(atoi(port.c_str())).multithreaded().run();
}