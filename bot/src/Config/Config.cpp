#include <bot/src/Config/Config.hpp>
#include <utils/Env/Env.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <charconv>
#include <map>
#include <algorithm>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Bot::Config {

using std::pair;
using std::to_string;
using std::ifstream;
using std::filesystem::exists;
using std::filesystem::is_regular_file;
using std::filesystem::file_size;
using std::ios;
using std::from_chars;
using std::errc;
using std::map;
using std::find_if;
using std::find;

#ifndef NDEBUG
using Utils::Logger::get_logger;
#endif

using nlohmann::json;

void throw_if_empty_config(string_view str, string_view config_name) {
    if (str.empty()) {
        throw ConfigNotFoundException(config_name);
    }
}

bool Config::is_help() const noexcept {
    return _is_help;
}

const string& Config::get_bot_token() const noexcept {
    return _bot_token;
}

const vector<long long>& Config::get_admins() const noexcept {
    return _admins;
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

int Config::get_listen_port() const noexcept {
    return _listen_port;
}

const string& Config::get_db_host() const noexcept {
    return _db_host;
}

int Config::get_db_port() const noexcept {
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

const string& Config::get_yt_service_url() const noexcept {
    return _yt_service_url;
}

const string& Config::get_task_service_url() const noexcept {
    return _task_service_url;
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

Config::Config(int argc, const char* argv[]) {

    #ifndef NDEBUG
    for (int i = 0; i < argc; i++) {
        get_logger()->debug("ARGV", argv[i], __FILE__, __LINE__);
    }
    #endif

    string ENV_FILE_KEY = "--env-file";
    string json_file_name;
    for (int i = 0; i < argc && json_file_name.empty(); i++) {
        string argument = argv[i];
        if (argument != ENV_FILE_KEY) {
            continue;
        }
        if (i+1 >= argc) {
            throw ConfigParseFailedException(ENV_FILE_KEY + ": value not found");
        }
        json_file_name = argv[i+1];
    }

    json json_env;

    if (!json_file_name.empty()) {
        if (!exists(json_file_name)) {
            throw ConfigParseFailedException(ENV_FILE_KEY + " (" + json_file_name + "): not found!");
        }
        if (!is_regular_file(json_file_name)) {
            throw ConfigParseFailedException(ENV_FILE_KEY + " (" + json_file_name + "): read access denied!");
        }
        
        ifstream json_file(json_file_name, ios::binary);
        auto size = file_size(json_file_name);
        string read(size, '\0');
        json_file.read(read.data(), size);

        if (!json::accept(read)) {
            throw ConfigParseFailedException(ENV_FILE_KEY + " (" + json_file_name + "): cannot parse json!");
        }
        json_env = json::parse(read);
        #ifndef NDEBUG
        get_logger()->debug("json_parse", json_env.dump(), __FILE__, __LINE__);
        #endif
    }

    struct ConfigParseParam {
        string param;
        string& config_value;
        bool is_string = true;
        bool is_throw = true;
    };

    ;

    if ((_is_help = find_if(argv, argv + argc, [](const char* argument) {
        static const vector<string> HELP_KEYS{"--help", "help"};
        return find(HELP_KEYS.begin(), HELP_KEYS.end(), argument) != HELP_KEYS.end();
    }) != (argv+argc))) {
        return;
    }
    
    string admins_str, db_port_str, listen_port_str;

    for (auto& config : vector<ConfigParseParam>{
        {"BOT_TOKEN", _bot_token},
        {"BOT_ADMINS", admins_str, false, true},
        {"WEBHOOK_PATH", _webhook_path},
        {"WEBHOOK_URL", _webhook_url},
        {"LISTEN_IP", _listen_ip},
        {"LISTEN_PORT", listen_port_str, false, false},
        {"DB_HOST", _db_host},
        {"DB_NAME", _db_name},
        {"DB_PORT", db_port_str, false, false},
        {"DB_PASSWORD", _db_password},
        {"YT_SERVICE_URL", _yt_service_url},
        {"TASK_SERVICE_URL", _task_service_url},
        {"TMP_PATH", _tmp_path},
        {"LOGS_PATH", _logs_path},
        {"SHARED_PATH", _shared_path},
    })  {
        #ifndef NDEBUG
        if (!config.config_value.empty()) {
            get_logger()->debug( config.param + "::Default", config.config_value, __FILE__, __LINE__);
        }
        #endif
        config.config_value = Utils::Env::Get(config.param, config.config_value);
        #ifndef NDEBUG
        get_logger()->debug(config.param + "::Env", config.config_value, __FILE__, __LINE__);
        #endif
        if (!json_env.empty() && json_env.contains(config.param)) {
            if (config.is_string) {
                config.config_value = string(json_env[config.param]);
            } else {
                config.config_value = to_string(json_env[config.param]);
            }
            #ifndef NDEBUG
            get_logger()->debug(config.param + "::JSON", config.config_value, __FILE__, __LINE__);
            #endif
        }
        string key = "--" + config.param;
        for (int i = 0; i < argc; i++) {
            if (key != argv[i]) {
                continue;
            } 
            if (i+1 >= argc) {
                throw ConfigParseFailedException(key + ": value not found");
            }
            config.config_value = argv[i+1];
            #ifndef NDEBUG
            get_logger()->debug(config.param + "::argv", config.config_value, __FILE__, __LINE__);
            #endif
            break;
        }
        if (config.config_value.empty() && config.is_throw) {
            throw ConfigNotFoundException(config.param);
        }
        #ifndef NDEBUG
        get_logger()->debug(config.param + "::RESULT", config.config_value, __FILE__, __LINE__);
        #endif
    }

    for (auto& config : map<string, pair<int&, string> >{
        {"LISTEN_PORT", {_listen_port, listen_port_str} },
        {"DB_PORT", {_db_port, db_port_str}}
    }) {
        string param = config.first;
        int& config_value = config.second.first;
        string config_str = config.second.second;
        if (!config_str.empty()) {
            auto [_, ec] = from_chars(config_str.data(), config_str.data() + config_str.size(), config_value);
            if (ec != errc()) {
                throw ConfigParseFailedException(param + ": invalid value (" + config_str + ")");
            }
        }
    }
    try {
        _admins = json::parse(admins_str).get<vector<long long> >();
    } catch (const json::parse_error&) {
        throw ConfigParseFailedException("BOT_ADMINS: invalid json (" + admins_str + ")");
    } catch (const json::type_error&) {
        throw ConfigParseFailedException("BOT_ADMINS: invalid type (" + admins_str + ")");
    }
}

}