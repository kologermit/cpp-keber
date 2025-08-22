#include <bot/Signal/Signal.hpp>
#include <bot/Server/InterfaceServer.hpp>
#include <utils/Logger/InterfaceLogger.hpp>

namespace Bot::Signal {

using Bot::Server::get_server;
using Utils::Logger::get_logger;

void signal_handler([[maybe_unused]]int signal) {
    get_logger()->info("SIGNAL_HANDLER", "STOP SERVER", __FILE__, __LINE__);
    get_server()->stop();
}

}