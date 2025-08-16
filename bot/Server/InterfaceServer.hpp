#pragma once

namespace Bot::Server {

struct InterfaceServer {

    virtual void run() noexcept = 0;

};

}