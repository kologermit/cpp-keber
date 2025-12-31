#include <utils/TGBotApi/Message/Keyboard/ReplyKeyboard.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <nlohmann/json.hpp>


namespace Utils::TGBotApi::Message::Keyboard {

using std::make_shared;
using Utils::TGBotApi::JSONKeys::KEYBOARD_KEY;
using Utils::TGBotApi::JSONKeys::RESIZE_KEYBOARD_KEY;
using Utils::TGBotApi::JSONKeys::REMOVE_KEYBOARD_KEY;
using Utils::TGBotApi::JSONKeys::ONE_TIME_KEYBOARD_KEY;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;

ReplyKeyboard::ReplyKeyboard(const json& json_keyboard) {
    for (const auto& json_lane : json_keyboard) {
        const auto end_iter = buttons.end();
        buttons.emplace_back();
        for (const auto& json_button : json_lane) {
            end_iter->push_back(make_shared<ReplyButton>(
                json_button[TEXT_KEY].get<string>()
            ));
        }
    }
}

ReplyKeyboard::ReplyKeyboard(vector<vector<shared_ptr<ReplyButton> > > buttons): 
buttons(buttons)
{}

string ReplyKeyboard::get_json() const noexcept {
    if (buttons.empty()) {
        return json{
            {REMOVE_KEYBOARD_KEY, true}
        }.dump();
    }
    json result = {
        {RESIZE_KEYBOARD_KEY, true},
        {ONE_TIME_KEYBOARD_KEY, false},
    };
    for (const auto& lane : buttons) {
        if (lane.empty()) {
            continue;
        }
        json lane_json;
        for (const auto& button : lane) {
            if (button == nullptr) {
                continue;
            }
            lane_json.push_back(button->get_json());
        }
        if (lane_json.empty()) {
            continue;
        }
        result[KEYBOARD_KEY].push_back(lane_json);
    }
    return result.dump();
}

}