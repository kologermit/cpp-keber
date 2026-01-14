#include <utils/BotApi/BotApi.hpp>

namespace Utils::BotApi {
    using std::make_unique;
    using httplib::Result;
    using nlohmann::json;
    using Utils::Api::throw_by_status_or_error;

    constexpr const char* RESULT_KEY = "result";
    constexpr const char* APPLICATION_JSON_KEY = "application/json";

    BotApi::BotApi(string_view base_url, string_view auth_key):
        InterfaceBotApi(),
        Api(base_url, auth_key)
    {}

    unique_ptr<User> BotApi::get_user(long long id) {
        const string path = fmt::format("/user/{}", id);
        const Result response = _client.Get(path);
        throw_by_status_or_error(_base_url, path, response);
        json json_result = json::parse(response->body)[RESULT_KEY];
        if (json_result.is_null()) {
            return nullptr;
        }
        return make_unique<User>(json_result);
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
        static const string path = "/message";
        const Result response = _client.Post(path, request_body.dump(), APPLICATION_JSON_KEY);
        throw_by_status_or_error(_base_url, path, response);
        return json::parse(response->body)[RESULT_KEY].get<long long>();
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

        static const string path = "/message";
        const Result response = _client.Patch(path, request_body.dump(), APPLICATION_JSON_KEY);
        throw_by_status_or_error(_base_url, path, response);
        return json::parse(response->body)["result"].get<long long>();
    }
}
