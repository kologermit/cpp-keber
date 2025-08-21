#include <bot/Entity/Message/MessageRepository.hpp>
#include <bot/Entity/User/User.hpp>
#include <bot/Entity/Chat/Chat.hpp>
#include <utils/Entity/exec_templates.hpp>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Bot::Entity::Message {

using Utils::Entity::exec_select_one;
using Utils::Entity::exec_insert;
using Utils::Entity::exec_update_by_id;
using Utils::Entity::create_rows_in_enum_table_if_empty;
using Utils::Entity::ID_COLUMN;
using Utils::Entity::DELETED_AT_COLUMN;
using Utils::Entity::DATETIME_FORMAT;
using Utils::Entity::FailedInsertException;
using Utils::Entity::FailedUpdateException;
using Bot::Entity::User::USERS_TABLE;
using Bot::Entity::Chat::CHATS_TABLE;
using pqxx::result;
using pqxx::row;
using pqxx::nontransaction;
using jed_utils::datetime;
using fmt::format;
using std::make_unique;

#ifndef NDEBUG
using Utils::Logger::get_logger;
#endif

MessageRepository::MessageRepository(connection& db):
_db(db) {
    create_rows_in_enum_table_if_empty(_db, MESSAGE_CONTENT_TYPES_TABLE, map_content_type_to_string);
}

unique_ptr<Message> MessageRepository::get_by_id(int id) {
    return exec_select_one<Message>(_db, MESSAGES_TABLE, {
        {ID_COLUMN, to_string(id)},
    });
}

unique_ptr<Message> MessageRepository::get_by_chat_id_and_telegram_id(long long chat_telegram_id, long long telegram_id) {
    nontransaction tx{_db};
    const string sql_query = format(
        "SELECT {}.* FROM {} JOIN {} ON {}.{} = {}.{} WHERE {}.{} = {} AND {}.{} = {}",
        MESSAGES_TABLE, MESSAGES_TABLE,
        CHATS_TABLE,
        MESSAGES_TABLE, CHAT_ID_COLUMN,
        CHATS_TABLE, ID_COLUMN,
        CHATS_TABLE, TELEGRAM_ID_COLUMN, tx.quote(chat_telegram_id),
        MESSAGES_TABLE, TELEGRAM_ID_COLUMN, tx.quote(telegram_id)
    );

    #ifndef NDEBUG
    get_logger()->debug("get_by_chat_id_and_telegram_id::sql", sql_query);
    #endif


    result res = tx.exec(sql_query);

    for (const row& r : res) {
        return make_unique<Message>(r);
    }

    return nullptr;
}

unique_ptr<Message> MessageRepository::get_by_telegram_message(const TGMessage& tg_message, int chat_id, int user_id, bool check_created) {
    unique_ptr<Message> reply_message;

    if (check_created) {
        auto message = get_by_chat_id_and_telegram_id(tg_message.chat->id, tg_message.id);
        if (message != nullptr) {
            return message;
        }
    }

    if (tg_message.reply_message != nullptr) {
        reply_message = get_by_telegram_message(*tg_message.reply_message, true, chat_id, user_id);
    }

    return create(Message(
        tg_message.id,
        tg_message.text,
        tg_message.file_download_id,
        tg_message.file_name,
        tg_message.file_content_type,
        chat_id,
        user_id,
        (reply_message == nullptr ? 0 : reply_message->id)
    ));
}

unique_ptr<Message> MessageRepository::create(const Message& message) {

    map<string, string> data = {
        {TELEGRAM_ID_COLUMN, to_string(message.telegram_id)},
        {TEXT_COLUMN, message.text},
        {FILE_DOWNLOAD_ID_COLUMN, message.file_download_id},
        {FILE_NAME_COLUMN, message.file_name}, 
        {FILE_CONTENT_TYPE_COLUMN, to_string(static_cast<int>(message.file_content_type))},
        {USER_ID_COLUMN, to_string(message.user_id)},
        {CHAT_ID_COLUMN, to_string(message.chat_id)},
    };

    if (message.reply_message_id >= 1) {
        data[REPLY_MESSAGE_ID_COLUMN] = to_string(message.reply_message_id);
    }

    return exec_insert<Message>(_db, MESSAGES_TABLE, data);

}

const string& throw_if_invalid_column_to_update(int column) {
    static const map<int, const string> map_column_to_name{
        {TELEGRAM_ID, TELEGRAM_ID_COLUMN},
        {TEXT, TEXT_COLUMN},
        {FILE_DOWNLOAD_ID, FILE_DOWNLOAD_ID_COLUMN},
        {FILE_NAME, FILE_NAME_COLUMN}, 
        {FILE_CONTENT_TYPE, FILE_CONTENT_TYPE_COLUMN}
    };

    auto find_iterator = map_column_to_name.find(column);

    if (find_iterator == map_column_to_name.end()) {
        throw FailedUpdateException("column cant be updated");
    }

    return find_iterator->second;
}

unique_ptr<Message> MessageRepository::update(const Message& message, vector<int> columns) {
    map<string, string> data;

    for (int column : columns) {
        data[throw_if_invalid_column_to_update(column)] = message.get_data_by_column(column);
    }

    return exec_update_by_id<Message>(_db, MESSAGES_TABLE, data, message.id);
}

unique_ptr<Message> MessageRepository::del(int id) {
    return exec_update_by_id<Message>(_db, MESSAGES_TABLE, {{DELETED_AT_COLUMN, datetime().to_string(DATETIME_FORMAT)}}, id);
}

}