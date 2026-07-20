#pragma once

#include <task_tracker/Config/InterfaceConfig.hpp>
#include <utils/Config/ConfigParser.hpp>
#include <vector>
#include <stdexcept>
#include <memory>

namespace TaskTracker::Config {

    using std::vector;
    using std::invalid_argument;
    using std::shared_ptr;
    using Utils::Config::Argument;

    struct Config final : InterfaceConfig {
        Config(const int argc, const char* argv[]);

        bool is_help() const noexcept override;
        const string& get_help() const noexcept override;
        void throw_if_has_exception() const override;
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
        string _help;
        shared_ptr<invalid_argument> _exception = nullptr;
        vector<Argument> _arguments;
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