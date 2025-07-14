#pragma once

#include <memory>
#include <string_view>

namespace Utils::Logger {

using std::shared_ptr;
using std::string_view;

enum EnumLoggerLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

struct InterfaceLogger {
    virtual void info(string_view event, string_view message) const {};
    virtual void warning(string_view event, string_view message) const {};
    virtual void error(string_view event, string_view message) const {};
    virtual void debug(string_view event, string_view message) const {};

    ~InterfaceLogger() = default;
};

shared_ptr<InterfaceLogger> get_logger(InterfaceLogger* logger = nullptr);


}
