#pragma once

#include <vector>
#include <utils/TGBotApi/InterfaceJSONEncode.hpp>
#include <utils/TGBotApi/Message/Keyboard/InterfaceReplyButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::shared_ptr;
using std::vector;

struct InterfaceReplyKeyboard : virtual InterfaceJSONEncode {

    virtual shared_ptr<vector<vector<shared_ptr<InterfaceReplyButton> > > > get_buttons() const noexcept = 0;

};

}