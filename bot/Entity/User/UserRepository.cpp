#include <bot/Entity/User/UserRepository.hpp>

namespace Bot::Entity::User {

    using Utils::Entity::create_rows_in_enum_table_if_empty;
    using Utils::Entity::exec_select_one;

    UserRepository::UserRepository(connection& db): InterfaceUserRepository(db)
    {
        create_rows_in_enum_table_if_empty(_db, USER_SCREENS_TABLE, map_user_screen_to_string);
    }

    unique_ptr<User> UserRepository::get_by_telegram_user(const TGUser& tg_user) {
        auto user = get_by_id(tg_user.id);
    
        if (user != nullptr) {
            return user;
        }
    
        return this->create(User(tg_user.name, tg_user.username, UserScreen::START, tg_user.id), true);
    }

    unique_ptr<User> UserRepository::get_by_username(string_view username) {
        return exec_select_one<User>(_db, User::get_table_name(), {{USERNAME_COLUMN, username.data()}});
    }
}