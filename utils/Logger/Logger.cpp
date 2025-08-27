#include <iostream>
#include <fstream>
#include <filesystem>
#include <utils/Logger/Logger.hpp>
#include <utils/Datetime.hpp>

namespace Utils::Logger {

using std::cout, std::clog, std::endl;
using std::lock_guard;
using jed_utils::datetime;
using Utils::Datetime::DATE_FORMAT;
using std::filesystem::exists;
using std::filesystem::is_directory;
using std::filesystem::create_directory;

Logger::Logger(string_view path, const EnumLoggerLevel& level):
_level(level)
{
    if (!exists(path)) {
        create_directory(path);
    }

    const string full_path = string(path) + (path.ends_with('/') ? "" : "/") + datetime().to_string(DATE_FORMAT) + ".txt";

    _file.open(full_path, std::ios::app);
}

void Logger::print_event(
        ostream& out, 
        [[maybe_unused]] const string_view file,
        [[maybe_unused]] const int line,
        const string_view level,
        const string_view event,
        const string_view message,
        const bool is_green,
        const bool is_yellow,
        const bool is_red
    ) const {
    lock_guard lg(_mutex);
    if (is_green) {
        out << "\033[32m";
    }
    if (is_yellow) {
        out << "\033[33m";
    }
    if (is_red) {
        out << "\033[31m";
    }
    out << "[" << datetime().to_string(DATETIME_FORMAT) << "][" << level << "]";
    #ifndef NDEBUG
    if (!file.empty() && line > 0) {
        out 
            << "[" << file
            << ":" << line << "]";
    }
    #endif
    out 
        << "[" << event << "]"
        << " -- " << message << endl;
    if (is_green || is_yellow || is_red) {
        out << "\033[0m";
    }
}


void Logger::debug(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::DEBUG) {
        print_event(cout, file, line, "DEBUG", event, message, false, true, false);
        print_event(_file, file, line, "DEBUG", event, message, false, false, false);
    }
}
void Logger::info(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::INFO) {
        print_event(cout, file, line, "INFO", event, message, true, false, false);
        print_event(_file, file, line, "INFO", event, message, false, false, false);
    }
}
void Logger::warning(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::WARNING) {
        print_event(cout, file, line, "WARNING", event, message, false, true, false);
        print_event(_file, file, line, "WARNING", event, message, false, false, false);
    }
}
void Logger::error(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::ERROR) {
        print_event(clog, file, line, "ERROR", event, message, false, false, true);
        print_event(_file, file, line, "ERROR", event, message, false, false, false);
    }
}

}
