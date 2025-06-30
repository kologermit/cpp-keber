#pragma once

#include <utils/Types.cpp>

namespace Utils {
namespace Logger {

using Utils::Types::ConstString;

class AbstractLogger {
    public:
        virtual void info(ConstString& event, ConstString& message) const = 0;
        virtual void warning(ConstString& event, ConstString& message) const = 0;
        virtual void error(ConstString& event, ConstString& message) const = 0;
        virtual void debug(ConstString& event, ConstString& message) const = 0;
};

}
}