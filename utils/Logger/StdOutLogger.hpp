#pragma once

#include <string_view>
#include <utils/Logger/InterfaceLogger.hpp>

namespace Utils::Logger {

struct StdOutLogger : InterfaceLogger {
    #ifdef NDEBUG
    explicit StdOutLogger(const EnumLoggerLevel& level=EnumLoggerLevel::DEBUG);
    #else
    explicit StdOutLogger(const EnumLoggerLevel& level=EnumLoggerLevel::INFO);
    #endif
    void debug(string_view event, string_view message) const override;
    void info(string_view event, string_view message) const override;
    void warning(string_view event, string_view message) const override;
    void error(string_view event, string_view message) const override;

    private:
    const EnumLoggerLevel _level;
};

}
