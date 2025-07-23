#pragma once

#include <utils/TGBotApi/Message/Keyboard/InterfaceReplyKeyboard.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

struct ReplyKeyboard : virtual InterfaceReplyKeyboard {

    const_string get_json() const noexcept override;
    ptrButtons get_buttons() const noexcept override;

    ReplyKeyboard(ptrButtons buttons = nullptr);

    private:

    ptrButtons _buttons;

};

}