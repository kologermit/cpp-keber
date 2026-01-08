#pragma once

#include <task_tracker/HTTPHandler/HandlerContext.hpp>
#include <task_tracker/GlobalContext.hpp>
#include <utils/HTTPServer/Server/InterfaceServer.hpp>

namespace TaskTracker::HTTPHandler {
    using std::shared_ptr;
    using Utils::HTTPServer::Server::InterfaceServer;
    using IServer = InterfaceServer<GlobalContext, HandlerContext>;
    void init_server(IServer& server);
}