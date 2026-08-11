#pragma once

#include <bot/GlobalContext.hpp>
#include <bot/MessageThread/InterfaceMessageThread.hpp>

namespace Bot::MessageThread {
    using std::stop_token; 

    struct MessageThread : InterfaceMessageThread {
        void run() override;
        void stop() override;

        private:
        static void _worker(stop_token st);
        jthread _thread;
    };
}