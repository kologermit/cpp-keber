#pragma once

#include <utils/TGBotApi/Message/Keyboard/InterfaceReplyKeyboard.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

struct ReplyKeyboard : virtual InterfaceReplyKeyboard {

    const_string get_json() const noexcept override;
    shared_ptr<vector<vector<shared_ptr<InterfaceReplyButton> > > > get_buttons() const noexcept override;

    ReplyKeyboard(const vector<vector<shared_ptr<InterfaceReplyButton> > >& buttons);

    private:

    shared_ptr<vector<vector<shared_ptr<InterfaceReplyButton> > > > _buttons;

};

}