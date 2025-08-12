#include <iostream>
#include <utils/Logger/StdOutLogger.hpp>
#include <datetime/datetime.h>

namespace Utils::Logger {

using std::cout, std::clog, std::endl;
using jed_utils::datetime;

StdOutLogger::StdOutLogger(const EnumLoggerLevel& level):
_level(level)
{}

void StdOutLogger::print_event(ostream& out, string_view file, int line, string_view level, string_view event, string_view message) const {
    out << "[" << datetime().to_string(DATETIME_FORMAT) << "][" << level << "]";
    bool is_debug = 
    #ifndef NDEBUG
    true
    #else
    false
    #endif
    ;
    if (!file.empty() && line > 0 && is_debug) {
        out 
            << "[" << file
            << ":" << line << "]";
    }
    out 
        << "[" << event << "]"
        << "-- " << message << endl;
}


void StdOutLogger::debug(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::DEBUG) {
        print_event(cout, file, line, "DEBUG", event, message);
    }
}
void StdOutLogger::info(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::INFO) {
        print_event(cout, file, line, "INFO", event, message);
    }
}
void StdOutLogger::warning(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::WARNING) {
        print_event(cout, file, line, "WARNING", event, message);
    }
}
void StdOutLogger::error(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::ERROR) {
        print_event(clog, file, line, "DEBUG", event, message);
    }
}

}
