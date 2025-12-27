#pragma once

#include <bot/HTTPHandler/HandlerContext.hpp>
#include <bot/GlobalContext.hpp>
#include <utils/HTTPServer/Server/InterfaceServer.hpp>

namespace Bot::HTTPHandler {
    using std::shared_ptr;
    using Utils::HTTPServer::Server::InterfaceServer;
    using IServer = InterfaceServer<GlobalContext, HandlerContext>;
    void init_server(IServer& server);
}