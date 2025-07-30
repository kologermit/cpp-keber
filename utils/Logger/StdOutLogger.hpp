#pragma once

#include <string_view>
#include <iostream>
#include <utils/Logger/InterfaceLogger.hpp>

namespace Utils::Logger {

using std::ostream;

struct StdOutLogger : InterfaceLogger {
    explicit StdOutLogger(const EnumLoggerLevel& level=
        #ifndef NDEBIG
        EnumLoggerLevel::DEBUG
        #else
        EnumLoggerLevel::INFO
        #endif
    );
    
    void debug(string_view file, int line, string_view event, string_view message) const override;
    void info(string_view file, int line, string_view event, string_view message) const override;
    void warning(string_view file, int line, string_view event, string_view message) const override;
    void error(string_view file, int line, string_view event, string_view message) const override;

    private:
    const EnumLoggerLevel _level;
    void print_event(ostream& out, string_view file, int line, string_view level, string_view event, string_view message) const;
};

}
