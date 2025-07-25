#include <utils/Logger/InterfaceLogger.hpp>
#include <stdexcept>

namespace Utils::Logger {

using std::runtime_error;
using std::shared_ptr;
using std::move;

shared_ptr<InterfaceLogger> get_logger(unique_ptr<InterfaceLogger> logger) { 
    static shared_ptr<InterfaceLogger> _logger = nullptr;
    if (_logger == nullptr && logger == nullptr) {
        throw runtime_error("Logger::get_logger -- logger not initialized!");
    }
    if (_logger == nullptr) {
        _logger = shared_ptr<InterfaceLogger>(move(logger));
    }
    return _logger; 
}

}