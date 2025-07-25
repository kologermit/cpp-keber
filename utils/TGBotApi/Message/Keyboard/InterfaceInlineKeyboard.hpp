#pragma once

#include <vector>
#include <utils/TGBotApi/Message/Keyboard/InterfaceInlineButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::shared_ptr;
using std::vector;

struct InterfaceInlineKeyboard : virtual InterfaceJSONEncode {

    virtual shared_ptr<vector<vector<shared_ptr<InterfaceInlineButton> > > > get_buttons() const noexcept = 0;

};

}