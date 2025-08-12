#pragma once

#include <bot/Handler/InterfaceBotHandler.hpp>
#include <bot/Handler/InterfaceRequestHandler.hpp>

namespace Bot::Server {

using Bot::Handler::InterfaceBotHandler;
using Bot::Handler::InterfaceRequestHandler;
using std::unique_ptr;

struct InterfaceServer {

    virtual void add_bot_handler(unique_ptr<InterfaceBotHandler> handler) noexcept = 0;
    virtual void add_request_handler(unique_ptr<InterfaceRequestHandler> handler) noexcept = 0;
    virtual void run() noexcept = 0;

};

}