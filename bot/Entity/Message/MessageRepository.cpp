#include <bot/Entity/Message/MessageRepository.hpp>
#include <bot/Entity/User/User.hpp>
#include <bot/Entity/Chat/Chat.hpp>
#include <utils/Entity/exec_templates.hpp>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Bot::Entity::Message {

using Utils::Entity::exec_select;
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
using pqxx::work;
using pqxx::result;
using pqxx::row;
using pqxx::nontransaction;
using jed_utils::datetime;
using fmt::format;
using std::make_unique;

#ifndef NDEBUG
using Utils::Logger::get_logger;
#endif

MessageRepository::MessageRepository(string_view conn_url):
_db(conn_url.data()) {
    create_rows_in_enum_table_if_empty(_db, MESSAGE_CONTENT_TYPES_TABLE, map_content_type_to_string);
}

unique_ptr<Message> MessageRepository::get_by_id(int id) {
    return exec_select<Message>(_db, MESSAGES_TABLE, {
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

    #ifndef NDENUG
    get_logger()->debug("get_by_chat_id_and_telegram_id::sql", sql_query);
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
        auto message = get_by_chat_id_and_telegram_id(tg_message.chat->id, tg_message.id);
        if (message != nullptr) {
            return message;
        }
    }

    if (tg_message.reply_message != nullptr) {
        reply_message = get_by_telegram_message(*tg_message.reply_message, true);
    }

    work tx{_db};

    const string sql_query = format(
        "WITH\
            user_cte AS (SELECT {} FROM {} WHERE {} = {}),\
            chat_cte AS (SELECT {} FROM {} WHERE {} = {}),\
            reply_message_cte AS (SELECT {} FROM {} WHERE {} = {}) \
        INSERT INTO {} ({}, {}, {}, {}, {}, {}, {}, {}) \
        SELECT \
            {}, {}, {}, {}, {},\
            (SELECT {} FROM user_cte),\
            (SELECT {} FROM chat_cte),\
            (SELECT {} FROM reply_message_cte LIMIT 1)\
        RETURNING *;",
        ID_COLUMN, USERS_TABLE, TELEGRAM_ID_COLUMN, tx.quote(tg_message.from->id),
        ID_COLUMN, CHATS_TABLE, TELEGRAM_ID_COLUMN, tx.quote(tg_message.chat->id),
        ID_COLUMN, MESSAGES_TABLE, TELEGRAM_ID_COLUMN, tx.quote(tg_message.reply_message != nullptr ? tg_message.reply_message->id : -1),
        MESSAGES_TABLE, TELEGRAM_ID_COLUMN, TEXT_COLUMN, FILE_DOWNLOAD_ID_COLUMN, FILE_NAME_COLUMN, FILE_CONTENT_TYPE_COLUMN, 
            USER_ID_COLUMN, CHAT_ID_COLUMN, REPLY_MESSAGE_ID_COLUMN,
        tx.quote(tg_message.id), tx.quote(tg_message.text), tx.quote(tg_message.file_download_id), tx.quote(tg_message.file_name), 
        tx.quote(static_cast<int>(tg_message.file_content_type)),
        ID_COLUMN,
        ID_COLUMN,
        ID_COLUMN
    );

    #ifndef NDEBUG
    get_logger()->debug("sql", sql_query, __FILE__, __LINE__);
    #endif

    tx.exec("ROLLBACK");
    tx.exec("BEGIN");

    result res = tx.exec(sql_query);

    tx.commit();

    for (const row& r : res) {
        return make_unique<Message>(r);
    }

    throw FailedInsertException("no one row");

}

unique_ptr<Message> MessageRepository::create(const Message& message) {

    map<string, string> data = {
        {TELEGRAM_ID_COLUMN, to_string(message.telegram_id)},
        {TEXT_COLUMN, message.text},
        {FILE_DOWNLOAD_ID_COLUMN, message.file_download_id},
        {FILE_NAME_COLUMN, message.file_name}, 
        {FILE_CONTENT_TYPE_COLUMN, to_string(message.file_content_type)},
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