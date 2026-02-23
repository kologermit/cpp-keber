#pragma once

#include <string>
#include <string_view>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/Message/Keyboard/Style.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string_view;
using std::string;
using nlohmann::json;

struct ReplyButton {

    json get_json() const noexcept;

    ReplyButton(string_view text, Style style = Style::WHITE);

    string text;
    Style style;

};

}