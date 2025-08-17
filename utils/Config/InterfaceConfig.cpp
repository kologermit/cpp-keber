#include <utils/Config/InterfaceConfig.hpp>

namespace Utils::Config {

using std::move;

ConfigNotFoundException::ConfigNotFoundException(string_view config_name): 
_message("Config " + string(config_name) + " not found") {}

const char* ConfigNotFoundException::what() const noexcept {
    return _message.data();
}

ConfigParseFailedException::ConfigParseFailedException(string_view message):
_message(message) {}

const char* ConfigParseFailedException::what() const noexcept {
    return _message.data();
}

NotInitializedConfigException::NotInitializedConfigException(string_view message):
_message(message) {}

const char* NotInitializedConfigException::what() const noexcept {
    return _message.data();
}

const InterfaceConfig* get_config(unique_ptr<InterfaceConfig> config) {
    static unique_ptr<InterfaceConfig> _config = nullptr;

    if (config != nullptr && _config == nullptr) {
        _config = move(config);
    }

    if (_config == nullptr) {
        throw NotInitializedConfigException("Bot::Config::get_config not initialized!");
    }

    return _config.get();
}

}