#include <bot/Entity/Callback/CallbackRepository.hpp>

namespace Bot::Entity::Callback {
    unique_ptr<Callback> CallbackRepository::get_by_telegram_callback(const TGCallback& tg_callback) {
        return create(Callback(
            tg_callback.data,
            tg_callback.message->id,
            tg_callback.from->id,
            tg_callback.message->chat->id,
            tg_callback.id
        ), true);
    }
}