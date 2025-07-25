#include <utils/TGBotApi/Message/Keyboard/InlineKeyboard.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using nlohmann::json;
using std::make_shared;
using Utils::TGBotApi::JSONKeys::INLINE_KEYBOARD_KEY;
using Utils::TGBotApi::JSONKeys::RESIZE_KEYBOARD_KEY;

InlineKeyboard::InlineKeyboard(const vector<vector<shared_ptr<InterfaceInlineButton> > >& buttons):
_buttons(make_shared<vector<vector<shared_ptr<InterfaceInlineButton> > > >(buttons))
{}

shared_ptr<vector<vector<shared_ptr<InterfaceInlineButton> > > > InlineKeyboard::get_buttons() const noexcept {
    return _buttons;
}

const_string InlineKeyboard::get_json() const noexcept {
    json result = {
        {RESIZE_KEYBOARD_KEY, true}
    };
    for (const auto& lane : *_buttons) {
        json lane_json;
        for (const auto& button : lane) {
            json button_json = json::parse(button->get_json());
            lane_json.push_back(button_json);
        }
        result[INLINE_KEYBOARD_KEY].push_back(lane_json);
    }
    return result.dump();
}

}