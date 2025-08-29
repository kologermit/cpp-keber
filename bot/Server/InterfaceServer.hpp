#pragma once

#include <memory>

namespace Bot::Server {

using std::shared_ptr;
using std::unique_ptr;

struct InterfaceServer {
    virtual ~InterfaceServer() = default;

    virtual void run() noexcept = 0;
    virtual void stop() = 0;

};

shared_ptr<InterfaceServer> get_server(unique_ptr<InterfaceServer> server = nullptr);

}