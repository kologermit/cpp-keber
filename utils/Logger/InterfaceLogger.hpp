#pragma once

#include <memory>
#include <exception>
#include <string_view>
#include <ostream>
#include <utils/Datetime.hpp>

namespace Utils::Logger {

using std::shared_ptr;
using std::string_view;
using std::unique_ptr;
using std::exception;
using std::string;
using Utils::Datetime::DATETIME_FORMAT;

enum EnumLoggerLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

using std::ostream;

struct InterfaceLogger {
    virtual void info(string_view event, string_view message, string_view file = "", int line = 0) const = 0;
    virtual void warning(string_view event, string_view message, string_view file = "", int line = 0) const = 0;
    virtual void error(string_view event, string_view message, string_view file = "", int line = 0) const = 0;
    virtual void debug(string_view event, string_view message, string_view file = "", int line = 0) const = 0;
    virtual void print_event(
        ostream& out, 
        string_view file, 
        int line, 
        string_view level, 
        string_view event, 
        string_view message, 
        bool is_green=false,
        bool is_yellow=false,
        bool is_red=false
    ) const = 0;

    ~InterfaceLogger() = default;
};

struct NotInitializedLoggerException : exception {

    NotInitializedLoggerException();

    const char* what() const noexcept override;

};

string get_filename(string_view file);
shared_ptr<InterfaceLogger> get_logger(unique_ptr<InterfaceLogger> logger = nullptr);


}
