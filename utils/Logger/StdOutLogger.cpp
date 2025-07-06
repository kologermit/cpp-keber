#pragma once

#include <iostream>
#include <fmt/core.h>
#include <utils/Logger/InterfaceLogger.cpp>
#include <utils/Logger/EnumLoggerLevel.cpp>
#include <utils/Type.cpp>

namespace Utils {
namespace Logger {

using namespace Utils::Type;
using namespace std;

class StdOutLogger : public virtual InterfaceLogger {
    private:
        const EnumLoggerLevel _level;
    public:
    StdOutLogger(const EnumLoggerLevel& level=EnumLoggerLevel::DEBUG):
    _level(level)
    {}
    void debug(const_string& event, const_string& message) const override {
        if (_level <= EnumLoggerLevel::DEBUG) {
            cout << "[DEBUG] [" << event << "] -- " << message << endl;
        }
    }
    void info(const_string& event, const_string& message) const override {
        if (_level <= EnumLoggerLevel::INFO) {
            cout << "[INFO] [" << event << "] -- " << message << endl;
        }
    }
    void warning(const_string& event, const_string& message) const override {
        if (_level <= EnumLoggerLevel::WARNING) {
            cout << "[WARNING] [" << event << "] -- " << message << endl;
        }
    }
    void error(const_string& event, const_string& message) const override {
        if (_level <= EnumLoggerLevel::ERROR) {
            cout << "[ERROR] [" << event << "] -- " << message << endl;
        }
    }
};

}
}