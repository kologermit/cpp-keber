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
using std::make_unique, std::move;

string CallbackQuery::get_id() const noexcept {
    return _id;
}

const InterfaceUser* CallbackQuery::get_from() const noexcept {
    return _from.get();
}

const InterfaceMessage* CallbackQuery::get_message() const noexcept {
    return _message.get();
};

string CallbackQuery::get_data() const noexcept {
    return _data;
}

CallbackQuery::CallbackQuery(const json& json_callback_query):
_id(json_callback_query[ID_KEY]),
_data(json_callback_query[DATA_KEY]),
_from(make_unique<User>(json_callback_query[FROM_KEY])),
_message(make_unique<Message>(json_callback_query[MESSAGE_KEY]))
{}

}