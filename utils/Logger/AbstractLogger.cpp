#pragma once

#include <utils/Types.cpp>

using Utils::Types::ConstString;

namespace Utils {
namespace Logger {

class AbstractLogger {
    public:
        virtual void info(ConstString& event, ConstString& message) const = 0;
        virtual void warning(ConstString& event, ConstString& message) const = 0;
        virtual void error(ConstString& event, ConstString& message) const = 0;
        virtual void debug(ConstString& event, ConstString& message) const = 0;
};

}
}