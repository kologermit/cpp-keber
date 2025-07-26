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
    string_view url,
    string_view callback_data
):
_text(text),
_url(url),
_callback_data(callback_data)
{}

string InlineButton::get_text() const noexcept {
    return _text;
}
string InlineButton::get_url() const noexcept {
    return _url;
}

string InlineButton::get_callback_data() const noexcept {
    return _callback_data;
}

string InlineButton::get_json() const noexcept {
    json result{
        {TEXT_KEY, _text},
    };
    if (!_url.empty()) {
        result[URL_KEY] = _url;
    }
    if (!_callback_data.empty()) {
        result[CALLBACK_DATA_KEY] = _callback_data;
    }
    return result.dump();
}

}