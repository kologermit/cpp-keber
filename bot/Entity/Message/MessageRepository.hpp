#pragma once

#include <bot/Entity/Message/InterfaceMessageRepository.hpp>
#include <pqxx/pqxx>

namespace Bot::Entity::Message {

    using std::string_view;
    using pqxx::connection;

    struct MessageRepository : InterfaceMessageRepository {
        explicit MessageRepository(connection& db);

        unique_ptr<Message> get_by_chat_and_id(long long chat_id, long long id) override;
        unique_ptr<Message> get_by_telegram_message(const TGMessage& tg_message) override;
    };

}