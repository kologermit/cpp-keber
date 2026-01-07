#pragma once

#include <bot/Config/InterfaceConfig.hpp>

namespace Bot::Config {

    struct Config final : InterfaceConfig {

        bool is_help() const noexcept override;
        const string& get_telegram_api_url() const noexcept override;
        const string& get_bot_token() const noexcept override;
        const vector<long long>& get_bot_admins() const noexcept override;
        const string& get_webhook_url() const noexcept override;
        const string& get_webhook_path() const noexcept override;
        const string& get_listen_ip() const noexcept override;
        long long get_listen_port() const noexcept override;
        const string& get_auth_key() const noexcept override;
        const string& get_db_host() const noexcept override;
        long long get_db_port() const noexcept override;
        const string& get_db_name() const noexcept override;
        const string& get_db_user() const noexcept override;
        const string& get_db_password() const noexcept override;
        const string& get_db_conn_url() const noexcept override;
        const string& get_rabbit_mq_host() const noexcept override;
        long long get_rabbit_mq_port() const noexcept override;
        const string& get_rabbit_mq_vhost() const noexcept override;
        const string& get_rabbit_mq_user() const noexcept override;
        const string& get_rabbit_mq_password() const noexcept override;
        const string& get_downloader_queue_name() const noexcept override;
        const string& get_youtube_api_url() const noexcept override;
        const string& get_task_tracker_url() const noexcept override;
        const string& get_tmp_path() const noexcept override;
        const string& get_logs_path() const noexcept override;
        const string& get_shared_path() const noexcept override;
        const string& get_file_buffer_path() const noexcept override;

        Config(const int argc, const char* argv[]);

        private:

        bool _is_help = false;
        string _telegram_api_url;
        string _bot_token;
        vector<long long> _bot_admins;
        string _webhook_url;
        string _webhook_path;
        string _listen_ip;
        long long _listen_port;
        string _auth_key;
        string _db_host;
        long long _db_port;
        string _db_name;
        string _db_user;
        string _db_password;
        string _db_conn_url;
        string _rabbit_mq_host;
        long long _rabbit_mq_port;
        string _rabbit_mq_vhost;
        string _rabbit_mq_user;
        string _rabbit_mq_password;
        string _downloader_queue_name;
        string _youtube_api_url;
        string _task_tracker_url;
        string _tmp_path;
        string _logs_path;
        string _shared_path;
        string _file_buffer_path;

    };

}