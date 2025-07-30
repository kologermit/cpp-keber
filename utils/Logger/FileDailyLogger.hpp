#pragma once

#include <spdlog/spdlog.h>
#include <utils/Types.hpp>
#include <utils/Logger/InterfaceLogger.hpp>

namespace Utils::Logger {

using spdlog::logger;
using std::string;

struct FileDailyLogger : InterfaceLogger {
    void info(string_view event, string_view message, string_view file, int line) const override;
    void warning(string_view event, string_view message, string_view file, int line) const override;
    void error(string_view event, string_view message, string_view file, int line) const override;
    void debug(string_view event, string_view message, string_view file, int line) const override;
    explicit FileDailyLogger(string_view dir);

    private:
    shared_ptr<logger> _logger;
    string get_text_from_message_and_event(string_view event, string_view message, string_view file = "", int line = 0) const;
};

}
