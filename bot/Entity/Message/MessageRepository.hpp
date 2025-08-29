#pragma once

#include <bot/Entity/Message/InterfaceMessageRepository.hpp>
#include <pqxx/pqxx>

namespace Bot::Entity::Message {

using std::string_view;
using pqxx::connection;

struct MessageRepository : InterfaceMessageRepository {

    unique_ptr<Message> get_by_id(int id) override;
    unique_ptr<Message> get_by_chat_id_and_telegram_id(long long chat_telegram_id, long long telegram_id) override;
    unique_ptr<Message> get_by_telegram_message(const TGMessage& tg_message, int user_id, int chat_id, bool check_created) override;
    unique_ptr<Message> create(const Message& message) override;
    unique_ptr<Message> update(const Message& message, vector<int> columns) override;
    unique_ptr<Message> del(int id) override;

    MessageRepository(connection& db);

    private:

    connection& _db;
};

}