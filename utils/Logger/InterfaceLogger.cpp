#include <utils/Logger/InterfaceLogger.hpp>
#include <stdexcept>

namespace Utils::Logger {

    using std::shared_ptr;
    using std::make_shared;
    using std::forward;

    const char* NotInitializedLoggerException::what() const noexcept {
        static const auto message = "logger not initialized!";
        return message;
    }

    shared_ptr<InterfaceLogger> get_logger(unique_ptr<InterfaceLogger> logger) {
        static shared_ptr<InterfaceLogger> _logger = nullptr;
        if (_logger == nullptr && logger == nullptr) {
            throw NotInitializedLoggerException();
        }
        if (_logger == nullptr) {
            _logger = std::move(logger);
        }
        return _logger;
    }

    string get_filename(string_view file) {
        string full_path(file);
        size_t find = full_path.rfind('/');
        if (find != std::string::npos) {
            return full_path.substr(find + 1);
        }
        return full_path;
    }
}