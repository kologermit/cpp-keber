#pragma once

#include <string_view>
#include <utils/Logger/InterfaceLogger.hpp>

namespace Utils::Logger {

struct StdOutLogger : virtual InterfaceLogger {
    explicit StdOutLogger(const EnumLoggerLevel& level=EnumLoggerLevel::DEBUG);
    void debug(string_view event, string_view message) const override;
    void info(string_view event, string_view message) const override;
    void warning(string_view event, string_view message) const override;
    void error(string_view event, string_view message) const override;

    protected:
    const EnumLoggerLevel _level;
};

}
