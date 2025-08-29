#include <bot/Entity/Repositories.hpp>
#include <bot/BotHandler/AccessHandler/AccessCallbackHandler.hpp>
#include <bot/BotHandler/AccessHandler/AccessHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <fmt/core.h>

namespace Bot::BotHandler::AccessHandler {

    using Bot::Entity::Repositories::get_repositories;
    using Bot::Entity::Access::EnumAccessType;
    using Bot::Entity::Access::Access;
    using Utils::TGBotApi::Bot::SendMessageParameters;
    using Utils::TGBotApi::Bot::get_bot;
    using fmt::format;
    using nlohmann::json;
    using std::stoi;
    using std::string_view;

    const string& AccessCallbackHandler::get_name() const noexcept {
        static const string name = "AccessCallbackHandler";
        return name;
    }

    bool AccessCallbackHandler::check(shared_ptr<BotHandlerContext> context) {
        return (context->access.full || context->access.access)
            && context->callback != nullptr
            && json::accept(context->callback->data)
            && json::parse(context->callback->data)[0].get<string>() == ACCESS_CALLBACK_NAME
        ;
    }

    ptrMessage AccessCallbackHandler::handle(shared_ptr<BotHandlerContext> context) {
        auto data = json::parse(context->callback->data);
        int user_id = data[1];
        auto access_type = static_cast<EnumAccessType>(data[2]);
        const bool access_value = data[3];
        const string word = data[4];

        auto user = get_repositories()->user_repository->get_by_id(user_id);
        if (user == nullptr) {
            get_bot()->answer_callback_query(
                context->callback->telegram_id,
                format(USER_NOT_FOUND_PHRASE, user_id),
                true
            );
            return nullptr;
        }

        get_bot()->answer_callback_query(context->callback->telegram_id);
        for (auto& access : get_repositories()->access_repository->get_raw_by_user_id(user->id)) {
            if (access->type == access_type) {
                get_repositories()->access_repository->del(access->id);
            }
        }

        if (access_value) {
            Access access;
            access.user_id = user->id;
            access.type = access_type;
            get_repositories()->access_repository->create(access);
        }

        try {
            get_bot()->send_message(SendMessageParameters{
                .chat_id = user->telegram_id,
                .text = (access_value
                    ? format(ADD_ACCESS_PHRASE, word, context->user->name)
                    : format(REMOVE_ACCESS_PHRASE, word, context->user->name)
                ),
            });
        } catch (...) {}

        return AccessHandler::send_message_with_access_keyboard(
            context,
            *user,
            get_repositories()->access_repository->get_by_user_id(user->id)
        );
    }
}