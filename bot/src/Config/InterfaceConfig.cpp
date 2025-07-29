#include <bot/src/Config/InterfaceConfig.hpp>

namespace Bot::Config {

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

}