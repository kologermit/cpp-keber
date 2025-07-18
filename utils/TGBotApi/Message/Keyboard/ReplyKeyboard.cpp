#include <utils/TGBotApi/Message/Keyboard/ReplyKeyboard.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <nlohmann/json.hpp>


namespace Utils::TGBotApi::Message::Keyboard {

using nlohmann::json;
using Utils::TGBotApi::JSONKeys::KEYBOARD_KEY;
using Utils::TGBotApi::JSONKeys::RESIZE_KEYBOARD_KEY;
using Utils::TGBotApi::JSONKeys::ONE_TIME_KEYBOARD_KEY;

ReplyKeyboard::ReplyKeyboard(ptrButtons buttons): _buttons(buttons) {}

ReplyKeyboard::ptrButtons ReplyKeyboard::get_buttons() const noexcept {
    return _buttons;
}

const_string ReplyKeyboard::get_json() const noexcept {
    json result = {
        {RESIZE_KEYBOARD_KEY, true},
        {ONE_TIME_KEYBOARD_KEY, false},
    };
    for (auto& lane : *_buttons) {
        json lane_json;
        for (auto& button : lane) {
            json button_json = json::parse(button->get_json());
            lane_json.push_back(button_json);
        }
        result[KEYBOARD_KEY].push_back(lane_json);
    }
    return result.dump();
}

}