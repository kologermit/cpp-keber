#include <bot/BotHandler/Access/AccessCallbackHandler.hpp>
#include <bot/BotHandler/Access/AccessHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <fmt/core.h>

namespace Bot::BotHandler::Access {

    using Bot::Entity::Access::EnumAccessType;
    using Bot::Entity::Access::Access;
    using Utils::TGBotApi::Bot::SendMessageParameters;
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
        long long user_id = data[1];
        auto access_type = static_cast<EnumAccessType>(data[2]);
        const bool access_value = data[3];
        const string word = data[4];

        auto user = context->global_context->user_repository->get_by_id(user_id);
        if (user == nullptr) {
            context->bot->answer_callback_query(
                context->callback->id,
                format(USER_NOT_FOUND_PHRASE, user_id),
                true
            );
            return nullptr;
        }

        context->bot->answer_callback_query(context->callback->id);
        for (auto& access : context->global_context->access_repository->get_raw_by_user_id(user->id)) {
            if (access->type == access_type) {
                context->global_context->access_repository->del(access->id);
            }
        }

        if (access_value) {
            context->global_context->access_repository->create(Access(
                user->id,
                access_type
            ));
        }

        try {
            context->bot->send_message(SendMessageParameters{
                .chat_id = user->id,
                .text = (access_value
                    ? format(ADD_ACCESS_PHRASE, word, context->user->name)
                    : format(REMOVE_ACCESS_PHRASE, word, context->user->name)
                ),
            });
        } catch (...) {}

        auto result_message = AccessHandler::send_message_with_access_keyboard(
            context,
            *user,
            context->global_context->access_repository->get_by_user_id(user->id)
        );

        return result_message;
    }
}