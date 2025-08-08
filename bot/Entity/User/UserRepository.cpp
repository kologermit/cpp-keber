#include <bot/Entity/User/UserRepository.hpp>
#include <utils/Entity/exec_templates.hpp>

namespace Bot::Entity::User {

using Utils::Entity::Exceptions::FailedUpdateException;
using Utils::Entity::create_rows_in_enum_table_if_empty;
using Utils::Entity::exec_insert;
using Utils::Entity::exec_update_by_id;
using Utils::Entity::exec_select;
using Utils::Entity::DELETED_AT_COLUMN;
using Utils::Entity::ID_COLUMN;
using Utils::Entity::DATETIME_FORMAT;

UserRepository::UserRepository(string_view conn_url):
_db(conn_url.data())
{
    create_rows_in_enum_table_if_empty(_db, USER_SCREENS_TABLE, map_user_screen_to_string);
}

unique_ptr<User> UserRepository::get_by_id(int id) {
    return exec_select<User>(_db, USERS_TABLE, {
        {ID_COLUMN, to_string(id)}
    });
}

unique_ptr<User> UserRepository::get_by_telegram_id(long long telegram_id) {
    return exec_select<User>(_db, USERS_TABLE, {
        {TELEGRAM_ID_COLUMN, to_string(telegram_id)}
    });
}

unique_ptr<User> UserRepository::get_by_telegram_user(const TGUser& tg_user) {
    auto user = this->get_by_telegram_id(tg_user.id);

    if (user != nullptr) {
        return user;
    }

    return this->create(User(
        tg_user.id,
        tg_user.name,
        tg_user.username,
        START
    ));
}

unique_ptr<User> UserRepository::create(const User& user) {
    return exec_insert<User>(_db, USERS_TABLE, map<string, string>{
        {TELEGRAM_ID_COLUMN, to_string(user.telegram_id)},
        {NAME_COLUMN, user.name},
        {USERNAME_COLUMN, user.username},
        {SCREEN_COLUMN, to_string(user.screen)}
    });
}

const string& throw_if_invalid_column_to_update(int column) {
    static const map<int, const string> map_column_to_name{
        {TELEGRAM_ID, TELEGRAM_ID_COLUMN},
        {NAME, NAME_COLUMN},
        {USERNAME, USERNAME_COLUMN},
        {SCREEN, SCREEN_COLUMN}
    };

    auto find_iterator = map_column_to_name.find(column);

    if (find_iterator == map_column_to_name.end()) {
        throw FailedUpdateException("column cant be updated");
    }

    return find_iterator->second;
}

unique_ptr<User> UserRepository::update(const User& user, vector<int> columns) {
    map<string, string> data;
    for (int column : columns) {
        data[throw_if_invalid_column_to_update(column)] = user.get_data_by_column(column);
    }
    
    return exec_update_by_id<User>(_db, USERS_TABLE, data, user.id);
}

unique_ptr<User> UserRepository::del(int id) {
    return exec_update_by_id<User>(_db, USERS_TABLE, {{DELETED_AT_COLUMN, datetime().to_string(DATETIME_FORMAT)}}, id);
}

}