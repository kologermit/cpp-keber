#include <utils/TGBotApi/CallbackQuery/CallbackQuery.hpp>
#include <utils/TGBotApi/User/User.hpp>
#include <utils/TGBotApi/Message/Message.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

namespace Utils::TGBotApi::CallbackQuery {

using Utils::TGBotApi::JSONKeys::ID_KEY;
using Utils::TGBotApi::JSONKeys::MESSAGE_KEY;
using Utils::TGBotApi::JSONKeys::FROM_KEY;
using Utils::TGBotApi::JSONKeys::DATA_KEY;
using Utils::TGBotApi::Message::Message;
using Utils::TGBotApi::User::User;
using std::make_shared;

const_string CallbackQuery::get_id() const noexcept {
    return _id;
}

shared_ptr<InterfaceUser> CallbackQuery::get_from() const noexcept {
    return _from;
}
shared_ptr<InterfaceMessage> CallbackQuery::get_message() const noexcept {
    return _message;
};
const_string CallbackQuery::get_data() const noexcept {
    return _data;
}

CallbackQuery::CallbackQuery(const json& json_callback_query):
_id(json_callback_query[ID_KEY]),
_message(make_shared<Message>(json_callback_query[MESSAGE_KEY])),
_from(make_shared<User>(json_callback_query[FROM_KEY])),
_data(json_callback_query[DATA_KEY])
{}

}