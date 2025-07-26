#pragma once

#include <spdlog/spdlog.h>
#include <utils/Types.hpp>
#include <utils/Logger/InterfaceLogger.hpp>

namespace Utils::Logger {

using spdlog::logger;
using std::string;

struct FileDailyLogger : InterfaceLogger {
    void info(string_view event, string_view message) const override;
    void warning(string_view event, string_view message) const override;
    void error(string_view event, string_view message) const override;
    void debug(string_view event, string_view message) const override;
    explicit FileDailyLogger();

    private:
    shared_ptr<logger> _logger;
    string get_text_from_message_and_event(string_view event, string_view message) const;
};

}
