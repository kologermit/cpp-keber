#pragma once

#include <utils/Logger/InterfaceLogger.hpp>

namespace Utils::Logger {

struct JSONSTDOutLogger : InterfaceLogger {

    #ifdef NDEBUG
    explicit JSONSTDOutLogger(const EnumLoggerLevel& level=EnumLoggerLevel::DEBUG);
    #else
    explicit JSONSTDOutLogger(const EnumLoggerLevel& level=EnumLoggerLevel::INFO);
    #endif

    void info(string_view event, string_view message) const override;
    void warning(string_view event, string_view message) const override;
    void error(string_view event, string_view message) const override;
    void debug(string_view event, string_view message) const override;

    private:
    const EnumLoggerLevel _level;

};

}