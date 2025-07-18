#pragma once

#include <vector>
#include <utils/TGBotApi/InterfaceJSONEncode.hpp>
#include <utils/TGBotApi/Message/Keyboard/InterfaceReplyButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::shared_ptr;
using std::vector;

struct InterfaceReplyKeyboard : virtual InterfaceJSONEncode {
    using Lane = vector<shared_ptr<InterfaceReplyButton> >;
    using Buttons = vector<Lane>;
    using ptrButtons = shared_ptr<Buttons>;
    virtual ptrButtons get_buttons() const noexcept = 0;

};

}