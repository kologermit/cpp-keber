#pragma once

#include <bot/Config/InterfaceConfig.hpp>

namespace Bot::Config {

struct Config : InterfaceConfig {

    bool                     is_help()              const noexcept override;
    const string&            get_bot_token()        const noexcept override;
    const vector<long long>& get_admins()           const noexcept override;
    const string&            get_webhook_url()      const noexcept override;
    const string&            get_webhook_path()     const noexcept override;
    const string&            get_listen_ip()        const noexcept override;
    int                      get_listen_port()      const noexcept override;
    const string&            get_db_host()          const noexcept override;
    int                      get_db_port()          const noexcept override;
    const string&            get_db_name()          const noexcept override;
    const string&            get_db_user()          const noexcept override;
    const string&            get_db_password()      const noexcept override;
    const string&            get_db_conn_url()      const noexcept override;
    const string&            get_yt_service_url()   const noexcept override;
    const string&            get_task_service_url() const noexcept override;
    const string&            get_tmp_path()         const noexcept override;
    const string&            get_logs_path()        const noexcept override;
    const string&            get_shared_path()      const noexcept override;

    Config(int argc, const char* argv[]);

    private:

    bool   _is_help = false;
    string _bot_token = "";
    vector<long long> _admins;
    string _webhook_url = "";
    string _webhook_path = "/webhook";
    string _listen_ip = "0.0.0.0";
    int    _listen_port = 8080;
    string _db_host = "localhost";
    int    _db_port = 5432;
    string _db_name = "bot";
    string _db_user = "bot";
    string _db_password = "qwerty";
    string _db_conn_url = "";
    string _yt_service_url = "yt";
    string _task_service_url = "task";
    string _tmp_path = "/tmp";
    string _logs_path = "./logs/bot";
    string _shared_path = "./shared";

};

}