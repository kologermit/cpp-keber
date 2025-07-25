#pragma once

#include <utils/TGBotApi/Message/Keyboard/InterfaceInlineKeyboard.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

struct InlineKeyboard : virtual InterfaceInlineKeyboard {

    const_string get_json() const noexcept override;
    shared_ptr<vector<vector<shared_ptr<InterfaceInlineButton> > > > get_buttons() const noexcept override;

    InlineKeyboard(const vector<vector<shared_ptr<InterfaceInlineButton> > >& _buttons);

    private:

    shared_ptr<vector<vector<shared_ptr<InterfaceInlineButton> > > > _buttons;

};

}