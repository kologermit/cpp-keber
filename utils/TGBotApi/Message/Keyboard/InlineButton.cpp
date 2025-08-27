#include <utils/TGBotApi/Message/Keyboard/InlineButton.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

namespace Utils::TGBotApi::Message::Keyboard {

using Utils::TGBotApi::JSONKeys::TEXT_KEY;
using Utils::TGBotApi::JSONKeys::URL_KEY;
using Utils::TGBotApi::JSONKeys::CALLBACK_DATA_KEY;

InlineButton::InlineButton(
    string_view text,
    string_view url,
    string_view callback_data
):
text(text),
url(url),
callback_data(callback_data)
{}

json InlineButton::get_json() const noexcept {
    json result{
        {TEXT_KEY, text},
    };
    if (!url.empty()) {
        result[URL_KEY] = url;
    }
    if (!callback_data.empty()) {
        result[CALLBACK_DATA_KEY] = callback_data;
    }
    return result;
}

}