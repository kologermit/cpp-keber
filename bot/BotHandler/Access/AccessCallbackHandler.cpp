#include <bot/BotHandler/Access/AccessCallbackHandler.hpp>
#include <bot/BotHandler/Access/AccessHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <fmt/core.h>

namespace Bot::BotHandler::Access {

    using Bot::Entity::Access::AccessType;
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

    bool AccessCallbackHandler::check(shared_ptr<BotHandlerContext> ctx) {
        return (ctx->access.full || ctx->access.access)
            && ctx->callback != nullptr
            && json::accept(ctx->callback->data)
            && json::parse(ctx->callback->data)[0].get<string>() == ACCESS_CALLBACK_NAME
        ;
    }

    ptrMessage AccessCallbackHandler::handle(shared_ptr<BotHandlerContext> ctx) {
        auto data = json::parse(ctx->callback->data);
        long long user_id = data[1];
        auto access_type = static_cast<AccessType>(data[2]);
        const bool access_value = data[3];
        const string word = data[4];

        auto user = ctx->db->user->get_by_id(user_id);
        if (user == nullptr) {
            ctx->bot->answer_callback_query(
                ctx->callback->id,
                format(USER_NOT_FOUND_PHRASE, user_id),
                true
            );
            return nullptr;
        }

        ctx->bot->answer_callback_query(ctx->callback->id);
        auto raw_user_access = ctx->db->access->get_raw_by_user_id(user->id);
        for (auto& access : *raw_user_access) {
            if (access.type == access_type) {
                ctx->db->access->del(access.id);
            }
        }

        if (access_value) {
            ctx->db->access->create(Access(
                user->id,
                access_type
            ));
        }

        try {
            ctx->bot->send_message(SendMessageParameters{
                .chat_id = user->id,
                .text = (access_value
                    ? format(ADD_ACCESS_PHRASE, word, ctx->user->name)
                    : format(REMOVE_ACCESS_PHRASE, word, ctx->user->name)
                ),
            });
        } catch (...) {}

        auto result_message = AccessHandler::send_message_with_access_keyboard(
            ctx,
            *user,
            ctx->db->access->get_by_user_id(user->id)
        );

        return result_message;
    }
}