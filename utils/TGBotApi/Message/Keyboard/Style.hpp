#pragma once

#include <string>

namespace Utils::TGBotApi::Message::Keyboard {
    enum Style {
        RED, GREEN, BLUE
    };
}

namespace std {
    using Utils::TGBotApi::Message::Keyboard::Style;
    const string& to_string(Style style);
}