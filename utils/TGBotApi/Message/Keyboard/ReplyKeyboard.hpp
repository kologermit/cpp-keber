#pragma once

#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/Message/Keyboard/ReplyButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using std::shared_ptr;
using std::vector;
using nlohmann::json;

struct ReplyKeyboard {

    string get_json() const noexcept;

    ReplyKeyboard(const json& json_keyboard);
    ReplyKeyboard(vector<vector<shared_ptr<ReplyButton> > > buttons = {});

    vector<vector<shared_ptr<ReplyButton> > > buttons;

};

}