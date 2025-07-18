#pragma once

#include <utils/TGBotApi/Message/Keyboard/InterfaceInlineButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string_view;
using std::nullopt;

struct InlineButton : virtual InterfaceInlineButton {

    const_string get_json() const noexcept override;
    const_string get_text() const noexcept override;
    optional_const_string get_url() const noexcept override;
    optional_const_string get_callback_data() const noexcept override;

    InlineButton(
        string_view text,
        optional_const_string url = nullopt,
        optional_const_string callback_data = nullopt
    );

    private:

    const_string _text;
    optional_const_string _url;
    optional_const_string _callback_data;
};

}