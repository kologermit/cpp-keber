#pragma once

#include <string>

namespace Utils::BotApi {
    using std::string;

    struct InlineButton {
        string text;
        string url;
        string callback_data;
    };
}