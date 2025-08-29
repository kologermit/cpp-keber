#include <bot/Entity/Callback/CallbackRepository.hpp>
#include <utils/Entity/exec_templates.hpp>

namespace Bot::Entity::Callback {

    using Utils::Entity::exec_select_one;
    using Utils::Entity::exec_insert;
    using Utils::Entity::ID_COLUMN;

    unique_ptr<Callback> CallbackRepository::get_by_id(int id) {
        return exec_select_one<Callback>(_db, CALLBACKS_TABLE, {{ID_COLUMN, to_string(id)}});
    }

    unique_ptr<Callback> CallbackRepository::get_by_telegram_id(string_view message) {
        return exec_select_one<Callback>(_db, CALLBACKS_TABLE, {{TELEGRAM_ID_COLUMN, string(message)}});
    }

    unique_ptr<Callback> CallbackRepository::get_by_telegram_callback(
        const TGCallback& tg_callback,
        int message_id,
        int user_id,
        int chat_id
    ) {
        return create(Callback(
            tg_callback.id,
            tg_callback.data,
            message_id,
            user_id,
            chat_id
        ));
    }

    unique_ptr<Callback> CallbackRepository::create(const Callback& callback) {
        return exec_insert<Callback>(_db, CALLBACKS_TABLE, {
            {TELEGRAM_ID_COLUMN, callback.telegram_id},
            {MESSAGE_ID_COLUMN, to_string(callback.message_id)},
            {DATA_COLUMN, callback.data},
            {CHAT_ID_COLUMN, to_string(callback.chat_id)},
            {USER_ID_COLUMN, to_string(callback.user_id)},
        });
    }

}