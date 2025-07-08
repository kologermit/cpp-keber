#pragma once

#include <iostream>
#include <string_view>
#include <fmt/core.h>
#include <utils/Logger/InterfaceLogger.cpp>
#include <utils/Logger/EnumLoggerLevel.cpp>
#include <utils/Type.cpp>

namespace Utils::Logger {

using Utils::Type::const_string;
using std::cout, std::endl, std::string_view;

struct StdOutLogger : virtual InterfaceLogger {
    StdOutLogger(const EnumLoggerLevel& level=EnumLoggerLevel::DEBUG):
    _level(level)
    {}
    void debug(string_view event, string_view message) const override {
        if (_level <= EnumLoggerLevel::DEBUG) {
            cout << "[DEBUG] [" << event << "] -- " << message << endl;
        }
    }
    void info(string_view event, string_view message) const override {
        if (_level <= EnumLoggerLevel::INFO) {
            cout << "[INFO] [" << event << "] -- " << message << endl;
        }
    }
    void warning(string_view event, string_view message) const override {
        if (_level <= EnumLoggerLevel::WARNING) {
            cout << "[WARNING] [" << event << "] -- " << message << endl;
        }
    }
    void error(string_view event, string_view message) const override {
        if (_level <= EnumLoggerLevel::ERROR) {
            cout << "[ERROR] [" << event << "] -- " << message << endl;
        }
    }

    protected:
        const EnumLoggerLevel _level;
};

}