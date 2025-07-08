#pragma once

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <utils/Logger/FileDailyLogger.hpp>

namespace Utils::Logger {

using spdlog::daily_logger_mt;
using spdlog::sinks::stdout_color_sink_mt;
using spdlog::level::level_enum;
using std::make_shared;
using std::map;
using fmt::format;

void FileDailyLogger::info(string_view event, string_view message) const {
    _logger->info(get_text_from_message_and_event(event, message));
}

void FileDailyLogger::warning(string_view event, string_view message) const {
    _logger->warn(get_text_from_message_and_event(event, message));
}

void FileDailyLogger::error(string_view event, string_view message) const {
    _logger->error(get_text_from_message_and_event(event, message));
}

void FileDailyLogger::debug(string_view event, string_view message) const {
    _logger->debug(get_text_from_message_and_event(event, message));
}

FileDailyLogger::FileDailyLogger() {
    const_string
        dir = "./logs",
        name = "main";
    EnumLoggerLevel level = EnumLoggerLevel::DEBUG;
    _logger = daily_logger_mt(name, dir, 0, 0);
    _logger->sinks().push_back(make_shared<stdout_color_sink_mt>());

    _logger->set_level(map<EnumLoggerLevel, level_enum> {
        {EnumLoggerLevel::DEBUG, level_enum::debug},
        {EnumLoggerLevel::INFO, level_enum::info},
        {EnumLoggerLevel::WARNING, level_enum::warn},
        {EnumLoggerLevel::ERROR, level_enum::err},
    }[level]);
}

const_string FileDailyLogger::get_text_from_message_and_event(string_view event, string_view message) const {
    return format(
        "[{}] -- {}",
        event,
        message
    );
};


}
