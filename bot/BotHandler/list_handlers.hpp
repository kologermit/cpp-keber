#pragma once

#include <vector>
#include <memory>
#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler {

    using std::vector;
    using std::shared_ptr;
    
    const vector<shared_ptr<InterfaceBotHandler> >& get_list_bot_handlers();
    
}