#include <iostream>
#include <utils/Logger/StdOutLogger.hpp>

namespace Utils::Logger {

using std::cout, std::clog, std::endl;

StdOutLogger::StdOutLogger(const EnumLoggerLevel& level):
_level(level)
{}

void StdOutLogger::print_event(ostream& out, string_view file, int line, string_view level, string_view event, string_view message) const {
    out 
        << "[" << get_filename(file)
        << "::" << line 
        << "::" << level 
        << "] [" << event 
        << "] -- " << message << endl;
}


void StdOutLogger::debug(string_view file, int line, string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::DEBUG) {
        print_event(cout, file, line, "DEBUG", event, message);
    }
}
void StdOutLogger::info(string_view file, int line, string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::INFO) {
        print_event(cout, file, line, "INFO", event, message);
    }
}
void StdOutLogger::warning(string_view file, int line, string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::WARNING) {
        print_event(cout, file, line, "WARNING", event, message);
    }
}
void StdOutLogger::error(string_view file, int line, string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::ERROR) {
        print_event(clog, file, line, "DEBUG", event, message);
    }
}

}
