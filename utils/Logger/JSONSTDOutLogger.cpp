#include <utils/Logger/JSONSTDOutLogger.hpp>
#include <utils/Types.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>

namespace Utils::Logger {

using std::cout, std::cerr, std::endl;
using json = nlohmann::ordered_json;
using parse_error = nlohmann::json::parse_error;
using Utils::Types::const_c_string;
using std::string, std::remove;
using std::ostream;

constexpr const_c_string LEVEL_KEY = "level";
constexpr const_c_string MESSAGE_KEY = "message";
constexpr const_c_string EVENT_KEY = "event";

struct TryParseResult {
    json json_result{};
    bool ok = false;
};

TryParseResult try_parse(string_view json_text) {
    try {
        return TryParseResult{
            .json_result = json::parse(json_text),
            .ok = true
        };
    } catch (const parse_error& e) {
        return TryParseResult {
            .ok = false
        };
    }
}

void print_event(ostream& stream, string_view level, string_view event, string_view message) {
    json json_data{
            {LEVEL_KEY, string(level)}, 
            {EVENT_KEY, string(event)}
        };
    auto parse_result = try_parse(message);
    if (parse_result.ok) {
        json_data[MESSAGE_KEY] = parse_result.json_result;
    } else {
        json_data[MESSAGE_KEY] = string(message);
    }
    string json_str = json_data.dump(1);
    json_str.erase(remove(json_str.begin(), json_str.end(), '\n'), json_str.end());
    stream << json_str << endl;
}

JSONSTDOutLogger::JSONSTDOutLogger(const EnumLoggerLevel& level) :
_level(level)
{}

void JSONSTDOutLogger::info(string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::INFO) {
        print_event(cout, "INFO", event, message);
    }
}

void JSONSTDOutLogger::warning(string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::WARNING) {
        print_event(cout, "WARNING", event, message);
    }
}

void JSONSTDOutLogger::error(string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::ERROR) {
        print_event(cerr, "ERROR", event, message);
    }
}

void JSONSTDOutLogger::debug(string_view event, string_view message) const {
    if (_level <= EnumLoggerLevel::DEBUG) {
        print_event(cout, "DEBUG", event, message);
    }
}

}