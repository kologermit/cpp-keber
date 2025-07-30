#pragma once

#include <utils/Logger/InterfaceLogger.hpp>
#include <iostream>

namespace Utils::Logger {

using std::ostream;

struct JSONSTDOutLogger : InterfaceLogger {

    explicit JSONSTDOutLogger(const EnumLoggerLevel& level=
        #ifndef NDEBUG
        EnumLoggerLevel::DEBUG
        #else
        EnumLoggerLevel::INFO
        #endif
    );

    void info(string_view event, string_view message, string_view file, int line) const override;
    void warning(string_view event, string_view message, string_view file, int line) const override;
    void error(string_view event, string_view message, string_view file, int line) const override;
    void debug(string_view event, string_view message, string_view file, int line) const override;

    private:
    const EnumLoggerLevel _level;

    void print_event(ostream& stream, string_view file, int line, string_view level, string_view event, string_view message) const;

};

}