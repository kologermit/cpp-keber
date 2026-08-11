#pragma once

namespace Bot::MessageThread {
    struct InterfaceMessageThread {
        virtual void run() = 0;
        virtual void stop() = 0;
        virtual ~InterfaceMessageThread() = default;
    };
}