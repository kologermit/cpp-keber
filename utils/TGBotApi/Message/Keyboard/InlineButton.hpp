#pragma once

#include <utils/TGBotApi/Message/Keyboard/InterfaceInlineButton.hpp>
#include <string_view>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string_view;
using std::string;

struct InlineButton : virtual InterfaceInlineButton {

    string get_json() const noexcept override;
    string get_text() const noexcept override;
    string get_url() const noexcept override;
    string get_callback_data() const noexcept override;

    InlineButton(
        string_view text,
        string_view url = "",
        string_view callback_data = ""
    );

    private:

    string _text;
    string _url;
    string _callback_data;
};

}