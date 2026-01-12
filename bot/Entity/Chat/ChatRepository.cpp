#include <bot/Entity/Chat/ChatRepository.hpp>
#include <utils/Entity/exec_templates.hpp>

namespace Bot::Entity::Chat {

    using Utils::Entity::create_rows_in_enum_table_if_empty;

    ChatRepository::ChatRepository(connection& db):
    InterfaceChatRepository(db) {
        create_rows_in_enum_table_if_empty(_db, CHAT_TYPES_TABLE, map_chat_type_to_string);
    }

    unique_ptr<Chat> ChatRepository::get_by_telegram_chat(const TGChat& tg_chat) {
        unique_ptr<Chat> chat = get_by_id(tg_chat.id);

        if (chat != nullptr) {
            return chat;
        }

        return create(Chat(
            tg_chat.name,
            tg_chat.username,
            tg_chat.type,
            tg_chat.id
        ), true);
    }
}