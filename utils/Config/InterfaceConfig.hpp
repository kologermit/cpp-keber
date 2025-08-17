#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <exception>
#include <memory>

namespace Utils::Config {

using std::string;
using std::vector;
using std::exception;
using std::string_view;
using std::unique_ptr;

struct ConfigNotFoundException : exception {
    ConfigNotFoundException(string_view config_name);

    const char* what() const noexcept override;

    private:

    string _message;

};

struct ConfigParseFailedException : exception {
    ConfigParseFailedException(string_view message);

    const char* what() const noexcept override;

    private:

    string _message;
};

struct NotInitializedConfigException : exception {
    NotInitializedConfigException(string_view message);

    const char* what() const noexcept override;

    private:

    string _message;
};

struct InterfaceConfig {

    virtual bool                     is_help()              const noexcept = 0;
    virtual const string&            get_bot_token()        const noexcept = 0;
    virtual const vector<long long>& get_admins()           const noexcept = 0;
    virtual const string&            get_webhook_url()      const noexcept = 0;
    virtual const string&            get_webhook_path()     const noexcept = 0;
    virtual const string&            get_listen_ip()        const noexcept = 0;
    virtual int                      get_listen_port()      const noexcept = 0;
    virtual const string&            get_db_host()          const noexcept = 0;
    virtual int                      get_db_port()          const noexcept = 0;
    virtual const string&            get_db_name()          const noexcept = 0;
    virtual const string&            get_db_user()          const noexcept = 0;
    virtual const string&            get_db_password()      const noexcept = 0;
    virtual const string&            get_db_conn_url()      const noexcept = 0;
    virtual const string&            get_yt_service_url()   const noexcept = 0;
    virtual const string&            get_task_service_url() const noexcept = 0;
    virtual const string&            get_tmp_path()         const noexcept = 0;
    virtual const string&            get_logs_path()        const noexcept = 0;
    virtual const string&            get_shared_path()      const noexcept = 0;

};

const InterfaceConfig* get_config(unique_ptr<InterfaceConfig> config = nullptr);

}