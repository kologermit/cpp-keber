#include <utils/Logger/InterfaceLogger.hpp>
#include <stdexcept>

namespace Utils::Logger {

using std::runtime_error;
using std::shared_ptr;
using std::move;

NotInitializedLoggerException::NotInitializedLoggerException() {}

const_c_string NotInitializedLoggerException::what() const noexcept {
    static const_c_string message = "logger not initialized!";
    return message;
}

shared_ptr<InterfaceLogger> get_logger(unique_ptr<InterfaceLogger> logger) { 
    static shared_ptr<InterfaceLogger> _logger = nullptr;
    if (_logger == nullptr && logger == nullptr) {
        throw NotInitializedLoggerException();
    }
    if (_logger == nullptr) {
        _logger = shared_ptr<InterfaceLogger>(move(logger));
    }
    return _logger; 
}

}