#pragma once

#include <utils/TGBotApi/InterfaceJSONEncode.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using Utils::Types::optional_const_string;
using Utils::Types::const_string;
using Utils::TGBotApi::JSONEncode::InterfaceJSONEncode;

struct InterfaceInlineButton : virtual InterfaceJSONEncode {

    virtual const_string get_text() const noexcept = 0;
    virtual optional_const_string get_url() const noexcept = 0;
    virtual optional_const_string get_callback_data() const noexcept = 0;

};

}