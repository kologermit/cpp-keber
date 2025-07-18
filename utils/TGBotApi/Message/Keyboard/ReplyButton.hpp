#pragma once

#include <utils/TGBotApi/Message/Keyboard/InterfaceReplyButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string_view;

struct ReplyButton : virtual InterfaceReplyButton {

    const_string get_json() const noexcept override;
    const_string get_text() const noexcept override;

    ReplyButton(string_view text);

    private:

    const_string _text;

};

}