#pragma once

#include <string_view>
#include <string>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/Message/Keyboard/Style.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string_view;
using std::string;
using nlohmann::json;

struct InlineButton {

    InlineButton(
        string_view text,
        string_view url = "",
        string_view callback_data = "",
        Style style = Style::WHITE
    );

    string text;
    string url;
    string callback_data;
    Style style;

    [[nodiscard]] json get_json() const noexcept;

};

}