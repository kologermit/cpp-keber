#pragma once 

#include <bot/Entity/Message/Message.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <vector>

namespace Bot::Entity::Message {

using TGMessage = Utils::TGBotApi::Types::Message;
using std::unique_ptr;
using std::vector;

struct InterfaceMessageRepository {
    virtual ~InterfaceMessageRepository() = default;

    virtual unique_ptr<Message> get_by_id(int id) = 0;
    virtual unique_ptr<Message> get_by_chat_id_and_telegram_id(long long chat_telegram_id, long long telegram_id) = 0;
    virtual unique_ptr<Message> get_by_telegram_message(const TGMessage& tg_message, int user_id, int chat_id, bool check_created = false) = 0;
    virtual unique_ptr<Message> create(const Message& message) = 0;
    virtual unique_ptr<Message> update(const Message& message, vector<int> columns) = 0;
    virtual unique_ptr<Message> del(int id) = 0;

};

}