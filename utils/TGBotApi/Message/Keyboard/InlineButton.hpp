#pragma once

#include <string_view>
#include <string>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string_view;
using std::string;

struct InlineButton {

    InlineButton(
        string_view text,
        string_view url = "",
        string_view callback_data = ""
    );

    string text;
    string url;
    string callback_data;

    string get_json() const noexcept;

};

}