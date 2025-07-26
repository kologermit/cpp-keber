#include <utils/TGBotApi/Message/Keyboard/ReplyButton.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using nlohmann::json;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;

ReplyButton::ReplyButton(string_view text): _text(text) {}

string ReplyButton::get_text() const noexcept {
    return _text;
}

string ReplyButton::get_json() const noexcept {
    return json{
        {TEXT_KEY, _text}
    }.dump();
}

}