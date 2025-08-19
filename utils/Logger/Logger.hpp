#pragma once

#include <string_view>
#include <iostream>
#include <fstream>
#include <mutex>
#include <utils/Logger/InterfaceLogger.hpp>

namespace Utils::Logger {

using std::ostream, std::ofstream;
using std::mutex;

struct Logger : InterfaceLogger {
    explicit Logger(
        string_view path="./logs/",
        const EnumLoggerLevel& level=
        #ifndef NDEBUG
        EnumLoggerLevel::DEBUG
        #else
        EnumLoggerLevel::INFO
        #endif
    );
    
    void debug(string_view event, string_view message, string_view file, int line) const override;
    void info(string_view event, string_view message, string_view file, int line) const override;
    void warning(string_view event, string_view message, string_view file, int line) const override;
    void error(string_view event, string_view message, string_view file, int line) const override;
    void print_event(
        ostream& out, 
        string_view file, 
        int line, 
        string_view level, 
        string_view event, 
        string_view message, 
        bool is_green=false,
        bool is_yellow=false,
        bool is_red=false
    ) const override;

    private:
    const EnumLoggerLevel _level;
    mutable ofstream _file;
    mutable mutex _mutex;

    
};

}
