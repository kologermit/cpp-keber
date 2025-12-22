#pragma once

#include <utils/HTTPServer/Handler/InterfaceHandler.hpp>
#include <vector>
#include <memory>

namespace Utils::HTTPServer::Server {
    using std::shared_ptr;
    using std::vector;
    using Utils::HTTPServer::Handler::InterfaceHandler;

    template <typename GlobalContext, typename HandlerContext>
    struct InterfaceServer {
        virtual void add_handler(shared_ptr<InterfaceHandler<HandlerContext> > handler) = 0;
        virtual void add_handlers(vector<shared_ptr<InterfaceHandler<HandlerContext> > > handlers) = 0;
        virtual void run() = 0;
        virtual void stop() = 0;
    };
}