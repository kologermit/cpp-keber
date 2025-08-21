#include <bot/Entity/Chat/ChatRepository.hpp>
#include <utils/Entity/exec_templates.hpp>

namespace Bot::Entity::Chat {

using Utils::Entity::FailedUpdateException;
using Utils::Entity::create_rows_in_enum_table_if_empty;
using Utils::Entity::exec_select_one;
using Utils::Entity::exec_insert;
using Utils::Entity::exec_update_by_id;
using Utils::Entity::ID_COLUMN;
using Utils::Entity::DELETED_AT_COLUMN;
using Utils::Entity::DATETIME_FORMAT;
using jed_utils::datetime;

ChatRepository::ChatRepository(connection& db):
_db(db) {
    create_rows_in_enum_table_if_empty(_db, CHAT_TYPES_TABLE, map_chat_type_to_string);
}

unique_ptr<Chat> ChatRepository::get_by_id(int id) {
    return exec_select_one<Chat>(_db, CHATS_TABLE, {
        {ID_COLUMN, to_string(id)}
    });
}

unique_ptr<Chat> ChatRepository::get_by_telegram_id(long long telegram_id) {
    return exec_select_one<Chat>(_db, CHATS_TABLE, {
        {TELEGRAM_ID_COLUMN, to_string(telegram_id)}
    });
}

unique_ptr<Chat> ChatRepository::get_by_telegram_chat(const TGChat& tg_chat) {
    auto chat = get_by_telegram_id(tg_chat.id);

    if (chat != nullptr) {
        return chat;
    }

    return exec_insert<Chat>(_db, CHATS_TABLE, {
        {TELEGRAM_ID_COLUMN, to_string(tg_chat.id)},
        {NAME_COLUMN, tg_chat.name},
        {USERNAME_COLUMN, tg_chat.username},
        {TYPE_COLUMN, to_string(tg_chat.type)},
    });
}

unique_ptr<Chat> ChatRepository::create(const Chat& chat) {
    return exec_insert<Chat>(_db, CHATS_TABLE, {
        {TELEGRAM_ID_COLUMN, to_string(chat.telegram_id)},
        {NAME_COLUMN, chat.name},
        {USERNAME_COLUMN, chat.username},
        {TYPE_COLUMN, to_string(chat.type)},
    });
}

const string& throw_if_invalid_column_to_update(int column) {
    static const map<int, const string> map_column_to_name{
        {TELEGRAM_ID, TELEGRAM_ID_COLUMN},
        {NAME, NAME_COLUMN},
        {USERNAME, USERNAME_COLUMN},
        {TYPE, TYPE_COLUMN}
    };

    auto find_iterator = map_column_to_name.find(column);

    if (find_iterator == map_column_to_name.end()) {
        throw FailedUpdateException("column cant be updated");
    }

    return find_iterator->second;
}

unique_ptr<Chat> ChatRepository::update(const Chat& chat, vector<int> columns) {
    map<string, string> data;
    for (int column : columns) {
        data[throw_if_invalid_column_to_update(column)] = chat.get_data_by_column(column);
    }
    return exec_update_by_id<Chat>(_db, CHATS_TABLE, data, chat.id);
}

unique_ptr<Chat> ChatRepository::del(int id) {
    return exec_update_by_id<Chat>(_db, CHATS_TABLE, {
        {DELETED_AT_COLUMN, datetime().to_string(DATETIME_FORMAT)}
    }, id);
}

}