#include <utils/TGBotApi/Message/Keyboard/ReplyButton.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using Utils::TGBotApi::JSONKeys::TEXT_KEY;
using Utils::TGBotApi::JSONKeys::STYLE_KEY;
using std::to_string;

ReplyButton::ReplyButton(
    string_view text,
    optional<Style> style
): text(text), style(style) {}

json ReplyButton::get_json() const noexcept {
    json result{{TEXT_KEY, text}};
    if (style.has_value()) {
        result[STYLE_KEY] = to_string(style.value());
    }
    return result;
}

}