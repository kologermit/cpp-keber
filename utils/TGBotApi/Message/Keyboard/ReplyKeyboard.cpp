#include <utils/TGBotApi/Message/Keyboard/ReplyKeyboard.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <nlohmann/json.hpp>


namespace Utils::TGBotApi::Message::Keyboard {

using std::make_shared;
using nlohmann::json;
using JSONKeys::KEYBOARD_KEY;
using JSONKeys::RESIZE_KEYBOARD_KEY;
using JSONKeys::REMOVE_KEYBOARD_KEY;
using JSONKeys::ONE_TIME_KEYBOARD_KEY;

ReplyKeyboard::ReplyKeyboard(vector<vector<shared_ptr<ReplyButton> > >&& buttons): 
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