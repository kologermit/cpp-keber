#include <bot/Entity/Message/MessageRepository.hpp>
#include <bot/Entity/User/User.hpp>
#include <bot/Entity/Chat/Chat.hpp>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Bot::Entity::Message {

    using Utils::Entity::create_rows_in_enum_table_if_empty;
    using Utils::Entity::ID;
    using Bot::Entity::Chat::Chat;
    using pqxx::result;
    using pqxx::row;
    using pqxx::nontransaction;
    using fmt::format;
    using std::make_unique;

    #ifndef NDEBUG
    using Utils::Logger::get_logger;
    #endif

    MessageRepository::MessageRepository(connection& db):
    InterfaceMessageRepository(db) {
        create_rows_in_enum_table_if_empty(_db, MESSAGE_CONTENT_TYPES_TABLE, map_content_type_to_string);
    }

    unique_ptr<Message> MessageRepository::get_by_chat_and_id(long long chat_id, long long id) {
        nontransaction tx{_db};

        const string sql_query = format(
            "SELECT {}.* FROM {} JOIN {} ON {}.{} = {}.{} WHERE {}.{} = {} AND {}.{} = {}",
            tx.quote_name(Message::get_table_name()),
            tx.quote_name(Message::get_table_name()),
            tx.quote_name(Chat::get_table_name()),
            tx.quote_name(Message::get_table_name()),
            tx.quote_name(CHAT_ID->name),
            tx.quote_name(Chat::get_table_name()),
            tx.quote_name(ID->name),
            tx.quote_name(Message::get_table_name()),
            tx.quote_name(ID->name),
            tx.quote(id),
            tx.quote_name(Chat::get_table_name()),
            tx.quote_name(ID->name),
            tx.quote(chat_id)
        );

        #ifndef NDEBUG
        get_logger()->debug("get_by_chat_id_and_telegram_id::sql", sql_query, __FILE__, __LINE__);
        #endif


        result res = tx.exec(sql_query);

        for (const row& r : res) {
            return make_unique<Message>(r);
        }

        return nullptr;
    }

    unique_ptr<Message> MessageRepository::get_by_telegram_message(const TGMessage& tg_message, bool check_created) {
        unique_ptr<Message> reply_message;

        if (check_created) {
            if (auto message = get_by_chat_and_id(tg_message.chat->id, tg_message.id); message != nullptr) {
                return message;
            }
        }

        if (tg_message.reply_message != nullptr) {
            reply_message = get_by_chat_and_id(tg_message.chat->id, tg_message.reply_message->id);
        }

        return create(Message(
            tg_message.text,
            tg_message.file_download_id,
            tg_message.file_name,
            tg_message.file_content_type,
            tg_message.chat->id,
            tg_message.from->id,
            (reply_message == nullptr ? 0 : reply_message->id),
            tg_message.id
        ));
    }
}