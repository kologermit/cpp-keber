#include <utils/TGBotApi/Message/Keyboard/ReplyButton.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using JSONKeys::TEXT_KEY;

ReplyButton::ReplyButton(string_view text): text(text) {}

json ReplyButton::get_json() const noexcept {
    return {
        {TEXT_KEY, text}
    };
}

}