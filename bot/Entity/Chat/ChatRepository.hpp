#pragma once

#include <bot/Entity/Chat/InterfaceChatRepository.hpp>

namespace Bot::Entity::Chat {
    struct ChatRepository : InterfaceChatRepository {
        unique_ptr<Chat> get_by_telegram_chat(const TGChat& tg_chat) override;

        explicit ChatRepository(connection& db);
    };
}