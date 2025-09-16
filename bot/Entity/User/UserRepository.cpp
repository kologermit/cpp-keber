#include <bot/Entity/User/UserRepository.hpp>

namespace Bot::Entity::User {

    using Utils::Entity::create_rows_in_enum_table_if_empty;
    using Utils::Entity::exec_insert;
    using Utils::Entity::exec_update_by_id;
    using Utils::Entity::exec_select_one;
    using Utils::Entity::DELETED_AT;
    using Utils::Entity::ID;
    using Utils::Entity::DATETIME_FORMAT;

    UserRepository::UserRepository(connection& db):
    Repository(db)
    {
        create_rows_in_enum_table_if_empty(_db, USER_SCREENS_TABLE, map_user_screen_to_string);
    }

    unique_ptr<User> UserRepository::get_by_id(int id) {
        return Repository::get_by_id(id);
    }

    // unique_ptr<User> UserRepository::get_by_telegram_user(const TGUser& tg_user) {
    //     auto user = this->get_by_id(tg_user.id);
    //
    //     if (user != nullptr) {
    //         return user;
    //     }
    //
    //     return this->create(User(
    //         tg_user.name,
    //         tg_user.username,
    //         START
    //     ));
    // }

    // unique_ptr<User> UserRepository::get_by_username(string_view username) {
    //     return exec_select_one<User>(_db, USERS_TABLE, {{USERNAME_COLUMN, username.data()}});
    // }


    unique_ptr<User> UserRepository::create(const User& user) {
        return exec_insert<User>(_db, user, true);
    }

    // const string& throw_if_invalid_column_to_update(int column) {
    //     static const map<int, const string> map_column_to_name{
    //         {TELEGRAM_ID, TELEGRAM_ID_COLUMN},
    //         {NAME, NAME_COLUMN},
    //         {USERNAME, USERNAME_COLUMN},
    //         {SCREEN, SCREEN_COLUMN}
    //     };
    //
    //     auto find_iterator = map_column_to_name.find(column);
    //
    //     if (find_iterator == map_column_to_name.end()) {
    //         throw FailedUpdateException("column cant be updated");
    //     }
    //
    //     return find_iterator->second;
    // }
    //
    // unique_ptr<User> UserRepository::update(const User& user, vector<int> columns) {
    //     map<string, string> data;
    //     for (int column : columns) {
    //         data[throw_if_invalid_column_to_update(column)] = user.get_data_by_column(column);
    //     }
    //
    //     return exec_update_by_id<User>(_db, USERS_TABLE, data, user.id);
    // }
    //
    // unique_ptr<User> UserRepository::del(int id) {
    //     return exec_update_by_id<User>(_db, USERS_TABLE, {{DELETED_AT_COLUMN, datetime().to_string(DATETIME_FORMAT)}}, id);
    // }

}