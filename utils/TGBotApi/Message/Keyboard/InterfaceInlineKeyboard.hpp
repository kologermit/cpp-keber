#pragma once

#include <vector>
#include <utils/TGBotApi/Message/Keyboard/InterfaceInlineButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::shared_ptr;
using std::vector;

struct InterfaceInlineKeyboard : virtual InterfaceJSONEncode {
    using Lane = vector<shared_ptr<InterfaceInlineButton> >;
    using Buttons = vector<Lane>;
    using ptrButtons = shared_ptr<Buttons>;
    
    virtual ptrButtons get_buttons() const noexcept = 0;

};

}