#pragma once

#include <bot/Entity/Chat/InterfaceChatRepository.hpp>

namespace Bot::Entity::Chat {

using std::string_view;
using pqxx::connection;

struct ChatRepository : InterfaceChatRepository {
    unique_ptr<Chat> get_by_id(int id) override;
    unique_ptr<Chat> get_by_telegram_id(long long telegram_id) override;
    unique_ptr<Chat> get_by_telegram_chat(const TGChat& tg_chat) override;
    unique_ptr<Chat> create(const Chat& chat) override;
    unique_ptr<Chat> update(const Chat& chat, vector<int> columns) override;
    unique_ptr<Chat> del(int id) override;

    ChatRepository(string_view conn_url);

    private:

    connection _db;
};

}