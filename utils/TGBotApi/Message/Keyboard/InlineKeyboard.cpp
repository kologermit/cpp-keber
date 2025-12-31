#include <utils/TGBotApi/Message/Keyboard/InlineKeyboard.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <fmt/core.h>

namespace Utils::TGBotApi::Message::Keyboard {

using std::invalid_argument;
using std::make_shared;
using Utils::TGBotApi::JSONKeys::INLINE_KEYBOARD_KEY;
using Utils::TGBotApi::JSONKeys::RESIZE_KEYBOARD_KEY;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;
using Utils::TGBotApi::JSONKeys::URL_KEY;
using Utils::TGBotApi::JSONKeys::CALLBACK_DATA_KEY;

InlineKeyboard::InlineKeyboard(const json& json_keyboard)
{
    for (const auto& json_lane : json_keyboard) {
        const auto end_iter = buttons.end();
        buttons.emplace_back();
        for (const auto& json_button : json_lane) {
            end_iter->push_back(make_shared<InlineButton>(
                json_button[TEXT_KEY].get<string>(),
                (json_button.contains(URL_KEY) ? json_button[URL_KEY].get<string>() : ""),
                (json_button.contains(CALLBACK_DATA_KEY) ? json_button[CALLBACK_DATA_KEY].get<string>() : "")
            ));
        }
    }
}

InlineKeyboard::InlineKeyboard(vector<vector<shared_ptr<InlineButton> > > buttons):
buttons(buttons)
{}

string InlineKeyboard::get_json() const noexcept {
    json result = {
        {RESIZE_KEYBOARD_KEY, true}
    };
    for (const auto& lane : buttons) {
        if (buttons.empty()) {
            continue;
        }
        json lane_json;
        for (const auto& button : lane) {
            if (button == nullptr) {
                continue;
            }
            lane_json.push_back(button->get_json());
        }
        result[INLINE_KEYBOARD_KEY].push_back(lane_json);
    }
    return result.dump();
}

}