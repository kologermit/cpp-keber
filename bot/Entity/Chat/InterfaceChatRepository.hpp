#pragma once

#include <bot/Entity/Chat/Chat.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <utils/Entity/Repository.hpp>
#include <memory>
#include <vector>

namespace Bot::Entity::Chat {

    using std::unique_ptr;
    using std::vector;
    using TGChat = Utils::TGBotApi::Types::Chat;
    using Utils::Entity::Repository;
    using pqxx::connection;

    struct InterfaceChatRepository : Repository<Chat> {
        virtual unique_ptr<Chat> get_by_telegram_chat(const TGChat& tg_user) = 0;
        explicit InterfaceChatRepository(connection& db): Repository(db) {}
    };
}