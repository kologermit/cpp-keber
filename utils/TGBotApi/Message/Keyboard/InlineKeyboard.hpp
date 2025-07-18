#pragma once

#include <utils/TGBotApi/Message/Keyboard/InterfaceInlineKeyboard.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

struct InlineKeyboard : virtual InterfaceInlineKeyboard {

    const_string get_json() const noexcept override;
    ptrButtons get_buttons() const noexcept override;

    InlineKeyboard(ptrButtons _buttons);

    private:

    ptrButtons _buttons;

};

}