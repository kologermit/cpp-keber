#include <utils/TGBotApi/CallbackQuery/CallbackQuery.hpp>
#include <utils/TGBotApi/User/User.hpp>
#include <utils/TGBotApi/Message/Message.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

#include "Logger/Logger.hpp"

namespace Utils::TGBotApi::CallbackQuery {

using Utils::TGBotApi::JSONKeys::ID_KEY;
using Utils::TGBotApi::JSONKeys::MESSAGE_KEY;
using Utils::TGBotApi::JSONKeys::FROM_KEY;
using Utils::TGBotApi::JSONKeys::DATA_KEY;
using Utils::TGBotApi::Message::Message;
using Utils::TGBotApi::User::User;
using std::make_unique, std::move;

CallbackQuery::CallbackQuery(const json& json_callback_query):
id(json_callback_query[ID_KEY]),
data(json_callback_query[DATA_KEY]),
from(
    json_callback_query.find(FROM_KEY) != json_callback_query.end()
    ? make_unique<User>(json_callback_query[FROM_KEY])
    : nullptr
),
message(
    json_callback_query.find(MESSAGE_KEY) != json_callback_query.end()
    ? make_unique<Message>(json_callback_query[MESSAGE_KEY])
    : nullptr
) {}

}
