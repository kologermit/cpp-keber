#include <utils/Logger/JSONSTDOutLogger.hpp>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <datetime/datetime.h>

namespace Utils::Logger {

using std::cout, std::clog, std::endl;
using json = nlohmann::ordered_json;
using parse_error = nlohmann::json::parse_error;
using std::string, std::remove, std::to_string;
using jed_utils::datetime;

constexpr const char* LEVEL_KEY = "level";
constexpr const char* FILE_KEY = "file";
constexpr const char* LINE_KEY = "line";
constexpr const char* MESSAGE_KEY = "message";
constexpr const char* EVENT_KEY = "event";
constexpr const char* TIME_KEY = "time";

void JSONSTDOutLogger::print_event(ostream& out, string_view file, int line, string_view level, string_view event, string_view message) const {
    json json_data{
        {TIME_KEY, datetime().to_string(DATETIME_FORMAT)},
        {LEVEL_KEY, string(level)},
    };
    if (!file.empty() && line > 0) {
        json_data[FILE_KEY] = string(file) + ":" + to_string(line);
    }
    json_data[EVENT_KEY] = event;
    if (json::accept(message)) {
        json_data[MESSAGE_KEY] = json::parse(message);
    } else {
        json_data[MESSAGE_KEY] = string(message);
    }
    string json_str = json_data.dump(1);
    json_str.erase(remove(json_str.begin(), json_str.end(), '\n'), json_str.end());
    out << json_str << endl;
}

JSONSTDOutLogger::JSONSTDOutLogger(const EnumLoggerLevel& level) :
_level(level)
{}

void JSONSTDOutLogger::info(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::INFO) {
        print_event(cout, file, line, "INFO", event, message);
    }
}

void JSONSTDOutLogger::warning(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::WARNING) {
        print_event(cout, file, line, "WARNING", event, message);
    }
}

void JSONSTDOutLogger::error(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::ERROR) {
        print_event(clog, file, line, "ERROR", event, message);
    }
}

void JSONSTDOutLogger::debug(string_view event, string_view message, string_view file, int line) const {
    if (_level <= EnumLoggerLevel::DEBUG) {
        print_event(cout, file, line, "DEBUG", event, message);
    }
}

}