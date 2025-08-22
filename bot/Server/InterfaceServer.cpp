#include <bot/Server/InterfaceServer.hpp>
#include <stdexcept>

namespace Bot::Server {

using std::move;
using std::runtime_error;

shared_ptr<InterfaceServer> get_server(unique_ptr<InterfaceServer> server) {
    static shared_ptr<InterfaceServer> _server;

    if (_server == nullptr && server == nullptr) {
        throw runtime_error("Bot::Server::get_server -- server not initialized!");
    }

    if (_server == nullptr) {
        _server = move(server);
    }

    return _server;
}

}