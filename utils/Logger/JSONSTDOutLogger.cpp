#include <utils/Logger/JSONSTDOutLogger.hpp>
#include <utils/Types.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <regex>

namespace Utils::Logger {

using std::cout, std::cerr, std::endl;
using json = nlohmann::ordered_json;
using Utils::Types::const_c_string;
using Utils::Types::const_string;
using std::string, std::remove;

constexpr const_c_string LEVEL_KEY = "level";
constexpr const_c_string MESSAGE_KEY = "message";
constexpr const_c_string EVENT_KEY = "event";

const_string json_dump_remove_endl(const json& json_data) {
    string json_str = json_data.dump(1);
    json_str.erase(remove(json_str.begin(), json_str.end(), '\n'), json_str.end());
    return json_str;
}

JSONSTDOutLogger::JSONSTDOutLogger(const EnumLoggerLevel& level) :
_level(level)
{}

void JSONSTDOutLogger::info(string_view event, string_view message) const {
    if (_level >= EnumLoggerLevel::INFO) {
        cout << json_dump_remove_endl({
            {LEVEL_KEY, "INFO"}, 
            {EVENT_KEY, event.data()}, 
            {MESSAGE_KEY, message.data()}
        }) << endl;
    }
}

void JSONSTDOutLogger::warning(string_view event, string_view message) const {
    if (_level >= EnumLoggerLevel::WARNING) {
        cout << json_dump_remove_endl({
            {LEVEL_KEY, "WARNING"}, 
            {EVENT_KEY, event.data()}, 
            {MESSAGE_KEY, message.data()}
        }) << endl;
    }
}

void JSONSTDOutLogger::error(string_view event, string_view message) const {
    if (_level >= EnumLoggerLevel::ERROR) {
        cerr << json_dump_remove_endl({
            {LEVEL_KEY, "ERROR"}, 
            {EVENT_KEY, event.data()}, 
            {MESSAGE_KEY, message.data()}
        }) << endl;
    }
}

void JSONSTDOutLogger::debug(string_view event, string_view message) const {
    if (_level >= EnumLoggerLevel::DEBUG) {
        cout << json_dump_remove_endl({
            {LEVEL_KEY, "DEBUG"}, 
            {EVENT_KEY, event.data()}, 
            {MESSAGE_KEY, message.data()}
        }) << endl;
    }
}

}