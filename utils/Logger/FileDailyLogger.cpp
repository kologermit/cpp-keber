#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <utils/Logger/FileDailyLogger.hpp>
#include <map>

namespace Utils::Logger {

using spdlog::daily_logger_mt;
using spdlog::sinks::stdout_color_sink_mt;
using spdlog::level::level_enum;
using std::make_shared;
using std::map;
using fmt::format;

void FileDailyLogger::info(string_view event, string_view message, string_view file, int line) const {
    _logger->info(get_text_from_message_and_event(event, message, file, line));
}

void FileDailyLogger::warning(string_view event, string_view message, string_view file, int line) const {
    _logger->warn(get_text_from_message_and_event(event, message, file, line));
}

void FileDailyLogger::error(string_view event, string_view message, string_view file, int line) const {
    _logger->error(get_text_from_message_and_event(event, message, file, line));
}

void FileDailyLogger::debug(string_view event, string_view message, string_view file, int line) const {
    _logger->debug(get_text_from_message_and_event(event, message, file, line));
}

FileDailyLogger::FileDailyLogger(string_view dir) {
    string name = "main";
    EnumLoggerLevel level = EnumLoggerLevel::DEBUG;
    _logger = daily_logger_mt(name, string(dir), 0, 0);
    _logger->sinks().push_back(make_shared<stdout_color_sink_mt>());

    _logger->set_level(map<EnumLoggerLevel, level_enum> {
        {EnumLoggerLevel::DEBUG, level_enum::debug},
        {EnumLoggerLevel::INFO, level_enum::info},
        {EnumLoggerLevel::WARNING, level_enum::warn},
        {EnumLoggerLevel::ERROR, level_enum::err},
    }[level]);
}

string FileDailyLogger::get_text_from_message_and_event(
    string_view event, string_view message,
    string_view file, int line
) const {
    if (!file.empty() && line > 0) {
        return format(
            "[{}::{}:{}] -- {}",
            event,
            file,
            line,
            message
        );
    }
    return format(
        "[{}] -- {}",
        event,
        message
    );
};


}
