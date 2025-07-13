#pragma once

#include <spdlog/spdlog.h>
#include <utils/Types.hpp>
#include <utils/Logger/InterfaceLogger.hpp>

namespace Utils::Logger {

using Utils::Types::const_string;
using spdlog::logger;

struct FileDailyLogger : virtual InterfaceLogger {
    void info(string_view event, string_view message) const override;
    void warning(string_view event, string_view message) const override;
    void error(string_view event, string_view message) const override;
    void debug(string_view event, string_view message) const override;
    explicit FileDailyLogger();

    protected:
    shared_ptr<logger> _logger;
    const_string get_text_from_message_and_event(string_view event, string_view message) const;
};

}
