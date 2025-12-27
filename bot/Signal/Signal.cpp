#include <bot/Signal/Signal.hpp>
#include <utils/Logger/InterfaceLogger.hpp>

namespace Bot::Signal {

using Utils::Logger::get_logger;

void signal_handler([[maybe_unused]]int signal) {
    get_logger()->info("SIGNAL_HANDLER", "STOP SERVER", __FILE__, __LINE__);
}

}