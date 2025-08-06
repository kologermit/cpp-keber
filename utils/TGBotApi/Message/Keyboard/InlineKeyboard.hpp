#pragma once

#include <vector>
#include <memory>
#include <utils/TGBotApi/Message/Keyboard/InlineButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {
    
using std::shared_ptr;
using std::vector;

struct InlineKeyboard {

    string get_json() const noexcept;

    InlineKeyboard(vector<vector<shared_ptr<InlineButton> > >&& buttons);

    vector<vector<shared_ptr<InlineButton> > > buttons;

};

}