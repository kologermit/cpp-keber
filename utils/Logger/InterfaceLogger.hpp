#pragma once

#include <memory>
#include <exception>
#include <string_view>
#include <utils/Types.hpp>

namespace Utils::Logger {

using std::shared_ptr;
using std::string_view;
using std::unique_ptr;
using std::exception;
using std::string;
using Utils::Types::const_c_string;

enum EnumLoggerLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

struct InterfaceLogger {
    virtual void info(string_view file, int line, string_view event, string_view message) const = 0;
    virtual void warning(string_view file, int line, string_view event, string_view message) const = 0;
    virtual void error(string_view file, int line, string_view event, string_view message) const = 0;
    virtual void debug(string_view file, int line, string_view event, string_view message) const = 0;

    ~InterfaceLogger() = default;
};

struct NotInitializedLoggerException : exception {

    NotInitializedLoggerException();

    const_c_string what() const noexcept override;

};

string get_filename(string_view file);
shared_ptr<InterfaceLogger> get_logger(unique_ptr<InterfaceLogger> logger = nullptr);


}
