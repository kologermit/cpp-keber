#pragma once

#include <string>
#include <string_view>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string_view;
using std::string;

struct ReplyButton {

    string get_json() const noexcept;

    ReplyButton(string_view text);

    string text;

};

}