#pragma once

#include <utils/TGBotApi/InterfaceJSONEncode.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::string;
using Utils::TGBotApi::JSONEncode::InterfaceJSONEncode;

struct InterfaceReplyButton : virtual InterfaceJSONEncode {

    virtual string get_text() const noexcept = 0;

};

}