#pragma once

#include <utils/TGBotApi/InterfaceJSONEncode.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using Utils::Types::const_string;
using Utils::TGBotApi::JSONEncode::InterfaceJSONEncode;

struct InterfaceReplyButton : virtual InterfaceJSONEncode {

    virtual const_string get_text() const noexcept = 0;

};

}