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
        virtual ~InterfaceServer() = default;

        using ptrIHandler = shared_ptr<InterfaceHandler<HandlerContext> >;
        virtual void add_handler(ptrIHandler handler) noexcept = 0;
        virtual void add_handlers(const vector<ptrIHandler>& handlers) noexcept = 0;
        virtual void run() = 0;
        virtual void stop() = 0;
    };
}