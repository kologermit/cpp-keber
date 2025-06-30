#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <utils/Types.cpp>
#include <utils/Env.cpp>
#include <utils/Logger/LoggerLevelEnum.cpp>
#include <utils/Logger/AbstractLogger.cpp>
#include <utils/Functions.cpp>

namespace Utils {
namespace Logger {

using Utils::Types::ConstString;
using Utils::Types::SharedPtr;
using Utils::Logger::LoggerLevelEnum;
using Utils::Logger::AbstractLogger;
using Utils::Functions::Format;

class FileDailyLogger : public AbstractLogger {

    private:
        SharedPtr<spdlog::logger> _logger;

        ConstString get_text_from_message_and_event(ConstString& event, ConstString& message) const {
            return Format(
                "[{}] -- {}",
                event,
                message
            );
        };

    public:

        void info(ConstString& event, ConstString& message) const override {
            this->_logger->info(this->get_text_from_message_and_event(event, message));
        }

        void warning(ConstString& event, ConstString& message) const override {
            this->_logger->warn(this->get_text_from_message_and_event(event, message));
        }

        void error(ConstString& event, ConstString& message) const override {
            this->_logger->error(this->get_text_from_message_and_event(event, message));
        }

        void debug(ConstString& event, ConstString& message) const override {
            this->_logger->debug(this->get_text_from_message_and_event(event, message));
        }

        FileDailyLogger() {
            ConstString
                dir = Utils::Env::Get("LOG_DIR", "./logs/"),
                name = "main";
            LoggerLevelEnum level = LoggerLevelEnum::DEBUG;
            this->_logger = spdlog::daily_logger_mt(name, dir, 0, 0);
            this->_logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            this->_logger->set_level(std::map<LoggerLevelEnum, spdlog::level::level_enum> {
                {LoggerLevelEnum::DEBUG, spdlog::level::debug},
                {LoggerLevelEnum::INFO, spdlog::level::info},
                {LoggerLevelEnum::WARN, spdlog::level::warn},
                {LoggerLevelEnum::ERR, spdlog::level::err},
            }[level]);
        }

};

}
}