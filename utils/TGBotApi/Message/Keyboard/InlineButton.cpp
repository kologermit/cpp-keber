#include <utils/TGBotApi/Message/Keyboard/InlineButton.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using nlohmann::json;
using Utils::TGBotApi::JSONKeys::TEXT_KEY;
using Utils::TGBotApi::JSONKeys::URL_KEY;
using Utils::TGBotApi::JSONKeys::CALLBACK_DATA_KEY;

InlineButton::InlineButton(
    string_view text,
    optional_const_string url,
    optional_const_string callback_data
):
_text(text),
_url(url),
_callback_data(callback_data)
{}

const_string InlineButton::get_text() const noexcept {
    return _text;
}
optional_const_string InlineButton::get_url() const noexcept {
    return _url;
}

optional_const_string InlineButton::get_callback_data() const noexcept {
    return _callback_data;
}

const_string InlineButton::get_json() const noexcept {
    json result{
        {TEXT_KEY, _text},
    };
    if (_url.has_value()) {
        result[URL_KEY] = *_url;
    }
    if (_callback_data.has_value()) {
        result[CALLBACK_DATA_KEY] = *_callback_data;
    }
    return result.dump();
}

}