#pragma once

#include <utils/TGBotApi/Message/Keyboard/InterfaceReplyButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string_view;

struct ReplyButton : virtual InterfaceReplyButton {

    string get_json() const noexcept override;
    string get_text() const noexcept override;

    ReplyButton(string_view text);

    private:

    string _text;

};

}