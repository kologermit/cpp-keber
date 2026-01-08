#include <utils/BotApi/BotApi.hpp>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::BotApi {
    #ifndef NDEBUG
    using Utils::Logger::get_logger;
    #endif
    using std::to_string;
    using std::make_unique;
    using std::runtime_error;
    using httplib::Headers;
    using httplib::Error;
    using httplib::Result;
    using httplib::Request;
    using nlohmann::json;

    BotApi::BotApi(
        string_view base_url,
        string_view auth_key
    ):
        _base_url(base_url),
        _auth_key(auth_key),
        _client(_base_url)
    {
        _client.set_default_headers(Headers{
            {"Authorization", _auth_key}
        });
    };

    void throw_by_status_or_error(const Result& response) {
        #ifndef NDEBUG
        get_logger()->debug("BotApi::Status", to_string(response->status), __FILE__, __LINE__);
        get_logger()->debug("BotApi::Body", response->body, __FILE__, __LINE__);
        #endif
        if (response.error() != Error::Success) {
            throw runtime_error(fmt::format("BotApi: error {}", to_string(response.error())));
        }
        if (response->status >= 300 || response->status < 200) {
            throw runtime_error(fmt::format("BotApi: status {}", response->status));
        }
        if (!json::accept(response->body)) {
            throw runtime_error("BotApi: body isn't json");
        }
    }

    unique_ptr<User> BotApi::get_user(long long id) {
        const Result response = _client.Get(fmt::format("/user/{}", id));
        throw_by_status_or_error(response);
        return make_unique<User>(json::parse(response->body)["result"]);
    }

    long long BotApi::post_message(
        optional<long long> chat_telegram_id,
        string_view text,
        string_view file_name,
        ContentType content_type,
        long long reply_to_message_id,
        const vector<vector<InlineButton> >& inline_keyboard,
        const vector<vector<string> >& reply_markup
    ) {
        json request_body{
            {"text", text},
        };
        if (chat_telegram_id.has_value()) {
            request_body["chat_telegram_id"] = chat_telegram_id.value();
        } else {
            request_body["is_admin"] = true;
        }
        if (!file_name.empty()) {
            request_body["file_name"] = file_name;
        }
        if (content_type != ContentType::UNKNOWN) {
            request_body["content_type"] = static_cast<int>(content_type);
        }
        if (reply_to_message_id != 0) {
            request_body["reply_to_message_id"] = reply_to_message_id;
        }
        if (!inline_keyboard.empty()) {
            request_body["inline_keyboard"] = json::array();
            for (const auto& lane : inline_keyboard) {
                request_body["inline_keyboard"].push_back(json::array());
                for (const auto& button : lane) {
                    (request_body["inline_keyboard"].end()-1)->push_back(json{
                        {"text", button.text},
                        {(!button.url.empty() ? "url" : "callback_data"), (!button.url.empty() ? button.url : button.callback_data)}
                    });
                }
            }
        }
        if (!reply_markup.empty()) {
            request_body["reply_markup"] = reply_markup;
        }
        const Result response = _client.Post("/message", request_body.dump(), "application/json");
        throw_by_status_or_error(response);
        return json::parse(response->body)["result"].get<long long>();
    }

    long long BotApi::patch_message(
        long long chat_telegram_id,
        long long message_id,
        string_view text,
        bool is_caption
    ) {
        json request_body{
            {"chat_telegram_id", chat_telegram_id},
            {"message_id", message_id},
            {(!is_caption ? "text" : "caption"), text},
        };

        const Result response = _client.Patch("/message", request_body.dump(), "application/json");
        throw_by_status_or_error(response);
        return json::parse(response->body)["result"].get<long long>();
    }
}
