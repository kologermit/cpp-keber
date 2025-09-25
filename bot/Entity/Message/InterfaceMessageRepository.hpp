#pragma once 

#include <bot/Entity/Message/Message.hpp>
#include <utils/Entity/Repository.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <vector>

namespace Bot::Entity::Message {

    using TGMessage = Utils::TGBotApi::Types::Message;
    using Utils::Entity::Repository;
    using pqxx::connection;
    using std::unique_ptr;

    struct InterfaceMessageRepository : Repository<Message> {
        InterfaceMessageRepository(connection& db): Repository(db) {}

        virtual unique_ptr<Message> get_by_chat_and_id(long long chat_id, long long id) = 0;
        virtual unique_ptr<Message> get_by_telegram_message(const TGMessage& tg_message, bool check_created = false) = 0;
    };

}