#pragma once

#include <string>
#include <string_view>
#include <nlohmann/json.hpp>
#include <utils/TGBotApi/User/User.hpp>
#include <utils/TGBotApi/Message/Message.hpp>

namespace Utils::TGBotApi::CallbackQuery {

using std::unique_ptr;
using std::string;
using std::string_view;
using nlohmann::json;
using Utils::TGBotApi::User::User;
using Utils::TGBotApi::Message::Message;

struct CallbackQuery {

    CallbackQuery(
        string_view id,
        string_view data = "",
        unique_ptr<User> from = nullptr,
        unique_ptr<Message> message = nullptr
    );

    CallbackQuery(const json& json_callback_query);

    string              id;
    string              data;
    unique_ptr<User>    from;
    unique_ptr<Message> message;

};

}