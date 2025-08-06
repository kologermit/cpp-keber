#pragma once

#include <vector>
#include <memory>
#include <utils/TGBotApi/Message/Keyboard/ReplyButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::shared_ptr;
using std::vector;

struct ReplyKeyboard {

    string get_json() const noexcept;

    ReplyKeyboard(vector<vector<shared_ptr<ReplyButton> > >&& buttons = {});

    vector<vector<shared_ptr<ReplyButton> > > buttons;

};

}