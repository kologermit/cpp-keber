#pragma once

#include <task_tracker/Config/InterfaceConfig.hpp>

namespace TaskTracker::Config {
    struct Config final : InterfaceConfig {
        Config(const int argc, const char* argv[]);

        bool is_help() const noexcept override;
        const string& get_auth_key() const noexcept override;
        const string& get_db_host() const noexcept override;
        long long get_db_port() const noexcept override;
        const string& get_db_name() const noexcept override;
        const string& get_db_user() const noexcept override;
        const string& get_db_password() const noexcept override;
        const string& get_db_conn_url() const noexcept override;
        const string& get_listen_ip() const noexcept override;
        long long get_listen_port() const noexcept override;
        const string& get_logs_path() const noexcept override;
        const string& get_bot_url() const noexcept override;
    private:
        bool _is_help;
        string _auth_key;
        string _db_host;
        long long _db_port;
        string _db_name;
        string _db_user;
        string _db_password;
        string _db_conn_url;
        string _listen_ip;
        long long _listen_port;
        string _logs_path;
        string _bot_url;
    };
}