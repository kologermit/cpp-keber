#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <utils/Type.cpp>
#include <utils/Logger/EnumLoggerLevel.cpp>
#include <utils/Logger/InterfaceLogger.cpp>
#include <memory>
#include <map>
#include <string_view>
#include <fmt/core.h>

namespace Utils::Logger {

using Utils::Type::const_string;
using spdlog::daily_logger_mt;
using spdlog::sinks::stdout_color_sink_mt;
using spdlog::level::level_enum;
using spdlog::logger;
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string_view;
using fmt::format;

struct FileDailyLogger : virtual InterfaceLogger {
    void info(string_view event, string_view message) const override {
        this->_logger->info(this->get_text_from_message_and_event(event, message));
    }

    void warning(string_view event, string_view message) const override {
        this->_logger->warn(this->get_text_from_message_and_event(event, message));
    }

    void error(string_view event, string_view message) const override {
        this->_logger->error(this->get_text_from_message_and_event(event, message));
    }

    void debug(string_view event, string_view message) const override {
        this->_logger->debug(this->get_text_from_message_and_event(event, message));
    }

    FileDailyLogger() {
        const_string
            dir = "./logs",
            name = "main";
        EnumLoggerLevel level = EnumLoggerLevel::DEBUG;
        this->_logger = daily_logger_mt(name, dir, 0, 0);
        this->_logger->sinks().push_back(make_shared<stdout_color_sink_mt>());

        this->_logger->set_level(map<EnumLoggerLevel, level_enum> {
            {EnumLoggerLevel::DEBUG, level_enum::debug},
            {EnumLoggerLevel::INFO, level_enum::info},
            {EnumLoggerLevel::WARNING, level_enum::warn},
            {EnumLoggerLevel::ERROR, level_enum::err},
        }[level]);
    }

    protected:
        shared_ptr<logger> _logger;

        const_string get_text_from_message_and_event(string_view event, string_view message) const {
            return format(
                "[{}] -- {}",
                event,
                message
            );
        };

};

}