#pragma once

#include <utils/Type.cpp>
#include <memory>

namespace Utils {
namespace Logger {

using namespace Utils::Type;
using namespace std;

struct InterfaceLogger {
    virtual void info(const_string& event, const_string& message) const = 0;
    virtual void warning(const_string& event, const_string& message) const = 0;
    virtual void error(const_string& event, const_string& message) const = 0;
    virtual void debug(const_string& event, const_string& message) const = 0;
};

shared_ptr<InterfaceLogger> GLOBAL_LOGGER;

}
}