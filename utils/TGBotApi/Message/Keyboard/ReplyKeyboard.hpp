#pragma once

#include <vector>
#include <memory>
#include <utils/TGBotApi/Message/Keyboard/InterfaceReplyKeyboard.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::shared_ptr;
using std::vector;

struct ReplyKeyboard : virtual InterfaceReplyKeyboard {

    string get_json() const noexcept override;

    ReplyKeyboard(vector<vector<shared_ptr<InterfaceReplyButton> > >&& buttons = {});

    private:

    vector<vector<shared_ptr<InterfaceReplyButton> > > _buttons;

};

}