#pragma once

#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/Message/Keyboard/InlineButton.hpp>

namespace Utils::TGBotApi::Message::Keyboard {
    
    using nlohmann::json;
    using std::shared_ptr;
    using std::vector;

    struct InlineKeyboard {

        string get_json() const noexcept;
        
        InlineKeyboard(const json& json_keyboard);
        InlineKeyboard(vector<vector<shared_ptr<InlineButton> > > buttons);

        vector<vector<shared_ptr<InlineButton> > > buttons;

    };

}