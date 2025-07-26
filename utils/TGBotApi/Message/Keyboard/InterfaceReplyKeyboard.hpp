#pragma once

#include <utils/TGBotApi/InterfaceJSONEncode.hpp>
#include <utils/TGBotApi/Message/Keyboard/InterfaceReplyButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

struct InterfaceReplyKeyboard : virtual InterfaceJSONEncode {};

}