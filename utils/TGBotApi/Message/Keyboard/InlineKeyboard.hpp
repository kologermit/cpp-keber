#pragma once

#include <vector>
#include <memory>
#include <utils/TGBotApi/Message/Keyboard/InterfaceInlineKeyboard.hpp>

namespace Utils::TGBotApi::Message::Keyboard {
    
using std::shared_ptr;
using std::vector;

struct InlineKeyboard : virtual InterfaceInlineKeyboard {

    string get_json() const noexcept override;

    explicit InlineKeyboard(vector<vector<shared_ptr<InterfaceInlineButton> > >&& _buttons);

    private:

    vector<vector<shared_ptr<InterfaceInlineButton> > > _buttons;

};

}