#pragma once

#include <string>

namespace TaskTracker::Config {
    using std::string;

    struct InterfaceConfig {
        virtual ~InterfaceConfig() = default;

        virtual bool is_help() const noexcept = 0;
        virtual const string&  get_auth_key() const noexcept = 0;
        virtual const string& get_db_host() const noexcept = 0;
        virtual long long get_db_port() const noexcept = 0;
        virtual const string& get_db_name() const noexcept = 0;
        virtual const string& get_db_user() const noexcept = 0;
        virtual const string& get_db_password() const noexcept = 0;
        virtual const string& get_db_conn_url() const noexcept = 0;
        virtual const string& get_listen_ip() const noexcept = 0;
        virtual long long get_listen_port() const noexcept = 0;
        virtual const string& get_logs_path() const noexcept = 0;
        virtual const string& get_bot_url() const noexcept = 0;
    };
}