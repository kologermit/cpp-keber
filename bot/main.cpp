#include <pqxx/pqxx>
#include <crow.h>
#include <fmt/core.h>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <hiredis/hiredis.h>

using namespace std;
using namespace std::chrono;
using namespace crow;
using namespace pqxx;

const char* get_env(const char* variable, const char* _default="") {
    const char* res = getenv(variable);
    if (res == nullptr) {
        return _default;
    }
    return res;
}

int main()
{
    const string
        db_host = get_env("DB_HOST", "localhost"),
        db_port = get_env("DB_PORT", "5432"),
        db_name = get_env("DB_NAME", "bot"),
        db_user = get_env("DB_USER", "postgres"),
        db_password = get_env("DB_PASSWORD", "qwerty"),
        cache_host = get_env("CACHE_HOST", "localhost"),
        cache_port = get_env("CACHE_PORT", "6379"),
        cache_password = get_env("CACHE_PASSWORD", "qwerty"),
        port = get_env("PORT", "8080");

    const auto db_conn_str = fmt::format(
        "host={} port={} dbname={} user={} password={}",
        db_host,
        db_port,
        db_name,
        db_user,
        db_password
    );
    cout << "DB CONN: " << db_conn_str << endl;




    auto cache_conn = redisConnect(cache_host.c_str(), atoi(cache_port.c_str()));
    auto reply = (redisReply*)redisCommand(cache_conn, "PING");
    if (reply == nullptr) {
        cerr << "Error" << std::endl;
        redisFree(cache_conn);
        return 1;
    }
    cout << "Ответ Redis: " << reply->str << std::endl;




    connection connectionObject(db_conn_str);
    work worker(connectionObject);


    SimpleApp app;
    CROW_ROUTE(app, "/")([&worker](){

        auto start = high_resolution_clock::now();

        string answer = "";

        for (auto row : worker.exec("\
            SELECT\
                \"id\",\
                \"name\",\
                \"screen\",\
                \"created\"\
            FROM \"user\"\
        ")) {
            const long id = row["id"].as<long>();
            const string
                name = row["name"].c_str(),
                screen = row["screen"].c_str(),
                created = row["created"].c_str();

            answer += fmt::format(
                "| id={} name={} screen={} created={}", 
                id, name, screen, created
            );
        }

        auto end = high_resolution_clock::now();

        duration<double, milli> duration = end - start;

        cout << "Request time: " << duration.count() << "ms" << endl;

        return answer;
    });

    cout << "Start cpp-keber" << endl;
    app.port(atoi(port.c_str())).multithreaded().run();
}