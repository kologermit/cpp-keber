#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <stdexcept>

namespace Utils::TGBotApi::Bot {

using std::runtime_error;
using std::move;

shared_ptr<InterfaceBot> get_bot(unique_ptr<InterfaceBot> bot) {
    static shared_ptr<InterfaceBot> _bot = nullptr;

    if (_bot == nullptr && bot == nullptr) {
        throw runtime_error("get_bot::not initialized!");
    }

    if (_bot == nullptr) {
        _bot = std::move(bot);
    }

    return _bot;
}

}