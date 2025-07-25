#include <iostream>
#include <utils/Logger/StdOutLogger.hpp>

namespace Utils::Logger {

using std::cout, std::cerr, std::endl;

StdOutLogger::StdOutLogger(const EnumLoggerLevel& level):
_level(level)
{}
void StdOutLogger::debug(string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::DEBUG) {
        cout << "[DEBUG] [" << event << "] -- " << message << endl;
    }
}
void StdOutLogger::info(string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::INFO) {
        cout << "[INFO] [" << event << "] -- " << message << endl;
    }
}
void StdOutLogger::warning(string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::WARNING) {
        cout << "[WARNING] [" << event << "] -- " << message << endl;
    }
}
void StdOutLogger::error(string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::ERROR) {
        cerr << "[ERROR] [" << event << "] -- " << message << endl;
    }
}

}
