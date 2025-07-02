#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <utils/Type.cpp>
#include <utils/Logger/EnumLoggerLevel.cpp>
#include <utils/Logger/InterfaceLogger.cpp>

namespace Utils {
namespace Logger {

using namespace Utils::Type;
using namespace Utils::Logger;
using namespace std;
using namespace spdlog;
using namespace fmt;

class FileDailyLogger : public InterfaceLogger {

    private:
        shared_ptr<logger> _logger;

        const_string get_text_from_message_and_event(const_string& event, const_string& message) const {
            return format(
                "[{}] -- {}",
                event,
                message
            );
        };

    public:

        void info(const_string& event, const_string& message) const override {
            this->_logger->info(this->get_text_from_message_and_event(event, message));
        }

        void warning(const_string& event, const_string& message) const override {
            this->_logger->warn(this->get_text_from_message_and_event(event, message));
        }

        void error(const_string& event, const_string& message) const override {
            this->_logger->error(this->get_text_from_message_and_event(event, message));
        }

        void debug(const_string& event, const_string& message) const override {
            this->_logger->debug(this->get_text_from_message_and_event(event, message));
        }

        FileDailyLogger() {
            const_string
                dir = "./logs",
                name = "main";
            EnumLoggerLevel level = EnumLoggerLevel::DEBUG;
            this->_logger = daily_logger_mt(name, dir, 0, 0);
            this->_logger->sinks().push_back(std::make_shared<sinks::stdout_color_sink_mt>());

            this->_logger->set_level(std::map<EnumLoggerLevel, level::level_enum> {
                {EnumLoggerLevel::DEBUG, level::debug},
                {EnumLoggerLevel::INFO, level::info},
                {EnumLoggerLevel::WARN, level::warn},
                {EnumLoggerLevel::ERR, level::err},
            }[level]);
        }

};

}
}