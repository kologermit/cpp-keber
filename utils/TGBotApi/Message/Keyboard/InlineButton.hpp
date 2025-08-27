#pragma once

#include <string_view>
#include <string>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string_view;
using std::string;
using nlohmann::json;

struct InlineButton {

    InlineButton(
        string_view text,
        string_view url = "",
        string_view callback_data = ""
    );

    string text;
    string url;
    string callback_data;

    [[nodiscard]] json get_json() const noexcept;

};

}