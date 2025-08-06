#include <utils/TGBotApi/Message/Keyboard/ReplyButton.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using nlohmann::json;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;

ReplyButton::ReplyButton(string_view text): text(text) {}

string ReplyButton::get_json() const noexcept {
    return json{
        {TEXT_KEY, text}
    }.dump();
}

}