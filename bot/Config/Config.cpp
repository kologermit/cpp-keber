#include <fmt/core.h>
#include <utils/Config/ConfigParser.hpp>
#include <bot/Config/Config.hpp>

namespace Bot::Config {
    using std::invalid_argument;
    using Utils::Config::parse_config;
    using Utils::Config::Argument;
    using Utils::Config::ArgumentType;

    Config::Config(const int argc, const char *argv[]) {
        vector<Argument> arguments{
            Argument{
                .name = "TELEGRAM_API_URL",
                .default_value = "https://api.telegram.org",
                .string_value = &_telegram_api_url,
            },
            Argument{
                .name = "BOT_TOKEN",
                .is_required = true,
                .string_value = &_bot_token,
            },
            Argument{
                .name = "BOT_ADMINS",
                .is_required = true,
                .type = ArgumentType::VECTOR_LONG_LONG,
                .vector_long_long_value = & _bot_admins,
            },
            Argument{
                .name = "WEBHOOK_URL",
                .is_required = true,
                .string_value = &_webhook_url,
            },
            Argument{
                .name = "WEBHOOK_PATH",
                .default_value = "/webhook",
                .string_value = &_webhook_path,
            },
            Argument{
                .name = "LISTEN_IP",
                .default_value = "0.0.0.0",
                .string_value = &_listen_ip,
            },
            Argument{
                .name = "LISTEN_PORT",
                .default_value = "8080",
                .type = ArgumentType::LONG_LONG,
                .long_long_value = &_listen_port,
            },
            Argument{
                .name = "AUTH_KEY",
                .default_value = "qwerty",
                .type = ArgumentType::STRING,
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
                .type = ArgumentType::LONG_LONG,
                .long_long_value = &_db_port,
            },
            Argument{
                .name = "DB_NAME",
                .default_value = "bot",
                .string_value = &_db_name,
            },
            Argument{
                .name = "DB_USER",
                .default_value = "bot",
                .string_value = &_db_user,
            },
            Argument{
                .name = "DB_PASSWORD",
                .default_value = "qwerty",
                .string_value = &_db_password,
            },
            Argument{
                .name = "RABBITMQ_HOST",
                .default_value = "localhost",
                .string_value = &_rabbit_mq_host,
            },
            Argument{
                .name = "RABBITMQ_PORT",
                .default_value = "15672",
                .type = ArgumentType::LONG_LONG,
                .long_long_value = &_rabbit_mq_port,
            },
            Argument{
                .name = "RABBITMQ_VHOST",
                .default_value = "/",
                .string_value = &_rabbit_mq_vhost,
            },
            Argument{
                .name = "RABBITMQ_USER",
                .default_value = "admin",
                .string_value = &_rabbit_mq_user,
            },
            Argument{
                .name = "RABBITMQ_PASSWORD",
                .default_value = "qwerty",
                .string_value = &_rabbit_mq_password,
            },
            Argument{
                .name = "DOWNLOADER_QUEUE_NAME",
                .default_value = "downloader",
                .string_value = &_downloader_queue_name,
            },
            Argument{
                .name = "YOUTUBE_API_URL",
                .default_value = "http://localhost:8081",
                .string_value = &_youtube_api_url,
            },
            Argument{
                .name = "TASK_TRACKER_URL",
                .default_value = "http://task_tracker:8082",
                .string_value = &_task_tracker_url,
            },
            Argument{
                .name = "TMP_PATH",
                .default_value = "./tmp",
                .string_value = &_task_tracker_url,
            },
            Argument{
                .name = "LOGS_PATH",
                .default_value = "./logs",
                .string_value = &_logs_path,
            },
            Argument{
                .name = "SHARED_PATH",
                .default_value = "./shared",
                .string_value = &_shared_path,
            },
            Argument{
                .name = "FILE_BUFFER_PATH",
                .default_value = "./file_buffer",
                .string_value = &_file_buffer_path,
            },
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

        if (_bot_admins.empty()) {
            throw invalid_argument("BOT_ADMINS array is empty!");
        }
    }

    bool Config::is_help() const noexcept {
        return _is_help;
    }

    const string &Config::get_telegram_api_url() const noexcept {
        return _telegram_api_url;
    }

    const string& Config::get_bot_token() const noexcept {
        return _bot_token;
    }

    const vector<long long>& Config::get_bot_admins() const noexcept {
        return _bot_admins;
    }

    const string& Config::get_webhook_url() const noexcept {
        return _webhook_url;
    }

    const string& Config::get_webhook_path() const noexcept {
        return _webhook_path;
    }

    const string& Config::get_listen_ip() const noexcept {
        return _listen_ip;
    }

    long long Config::get_listen_port() const noexcept {
        return _listen_port;
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

    const string& Config::get_rabbit_mq_host() const noexcept {
        return _rabbit_mq_host;
    }

    long long Config::get_rabbit_mq_port() const noexcept {
        return _rabbit_mq_port;
    }

    const string& Config::get_rabbit_mq_vhost() const noexcept {
        return _rabbit_mq_vhost;
    }

    const string& Config::get_rabbit_mq_user() const noexcept {
        return _rabbit_mq_user;
    }

    const string& Config::get_rabbit_mq_password() const noexcept {
        return _rabbit_mq_password;
    }

    const string& Config::get_downloader_queue_name() const noexcept {
        return _downloader_queue_name;
    }

    const string& Config::get_youtube_api_url() const noexcept {
        return _youtube_api_url;
    }

    const string& Config::get_task_tracker_url() const noexcept {
        return _task_tracker_url;
    }

    const string& Config::get_tmp_path() const noexcept {
        return _tmp_path;
    }

    const string& Config::get_logs_path() const noexcept {
        return _logs_path;
    }

    const string& Config::get_shared_path() const noexcept {
        return _shared_path;
    }

    const string& Config::get_file_buffer_path() const noexcept {
        return _file_buffer_path;
    }
}
