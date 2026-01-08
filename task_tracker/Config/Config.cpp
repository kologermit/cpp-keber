#include <task_tracker/Config/Config.hpp>
#include <utils/Config/ConfigParser.hpp>
#include <fmt/core.h>

namespace TaskTracker::Config {
    using Utils::Config::Argument;
    using Utils::Config::parse_config;
    using std::vector;

    Config::Config(const int argc, const char *argv[]) {
        vector<Argument> arguments{
            Argument{
                .name = "AUTH_KEY",
                .default_value = "qwerty",
                .string_value = &_auth_key,
            },
            Argument{
                .name = "DB_HOST",
                .default_value = "localhost",
                .string_value = &_db_host,
            },
            Argument{
                .name = "DB_PORT",
                .default_value = "5432",
                .long_long_value = &_db_port,
            },
            Argument{
                .name = "DB_NAME",
                .default_value = "bot",
                .string_value = &_db_name,
            },
            Argument{
                .name = "DB_USER",
                .default_value = "task_tracker",
                .string_value = &_db_user,
            },
            Argument{
                .name = "DB_PASSWORD",
                .default_value = "qwerty",
                .string_value = &_db_password,
            },
            Argument{
                .name = "LISTEN_IP",
                .default_value = "0.0.0.0",
                .string_value = &_listen_ip,
            },
            Argument{
                .name = "LISTEN_PORT",
                .default_value = "8080",
                .long_long_value = &_listen_port,
            },
            Argument{
                .name = "LOGS_PATH",
                .default_value = "./logs",
                .string_value = &_logs_path,
            },
            Argument{
                .name = "BOT_URL",
                .default_value = "http://localhost:8080",
                .string_value = &_bot_url,
            }
        };
        _is_help = parse_config(argc, argv, arguments);
        _db_conn_url = fmt::format(
            "host={} port={} dbname={} user={} password={}",
            _db_host,
            _db_port,
            _db_name,
            _db_user,
            _db_password
        );
    }

    bool Config::is_help() const noexcept {
        return _is_help;
    }

    const string& Config::get_auth_key() const noexcept {
        return _auth_key;
    }

    const string& Config::get_db_host() const noexcept {
        return _db_host;
    }

    long long Config::get_db_port() const noexcept {
        return _db_port;
    }

    const string& Config::get_db_name() const noexcept {
        return _db_name;
    }

    const string& Config::get_db_user() const noexcept {
        return _db_user;
    }

    const string& Config::get_db_password() const noexcept {
        return _db_password;
    }

    const string& Config::get_db_conn_url() const noexcept {
        return _db_conn_url;
    }

    const string& Config::get_listen_ip() const noexcept {
        return _listen_ip;
    }

    long long Config::get_listen_port() const noexcept {
        return _listen_port;
    }

    const string& Config::get_logs_path() const noexcept {
        return _logs_path;
    }

    const string& Config::get_bot_url() const noexcept {
        return _bot_url;
    }
}
