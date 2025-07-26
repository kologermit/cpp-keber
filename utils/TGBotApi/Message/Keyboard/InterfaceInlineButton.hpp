#pragma once

#include <utils/TGBotApi/InterfaceJSONEncode.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string;
using Utils::TGBotApi::JSONEncode::InterfaceJSONEncode;

struct InterfaceInlineButton : virtual InterfaceJSONEncode {

    virtual inline string get_text() const noexcept = 0;
    virtual inline string get_url() const noexcept = 0;
    virtual inline string get_callback_data() const noexcept = 0;

};

}