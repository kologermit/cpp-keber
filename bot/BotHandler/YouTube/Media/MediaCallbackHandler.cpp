#include <bot/BotHandler/YouTube/Media/MediaCallbackHandler.hpp>
#include <bot/BotHandler/YouTube/Media/MediaHandler.hpp>
#include <bot/BotHandler/YouTube/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <utils/JSONKeys.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/TGBotApi/Types.hpp>

namespace Bot::BotHandler::YouTube::Media {

    using nlohmann::json;
    using Utils::TGBotApi::JSONKeys::DATA_KEY;
    using Utils::TGBotApi::JSONKeys::CALLBACK_DATA_KEY;
    using Utils::TGBotApi::JSONKeys::CALLBACK_QUERY_ID_KEY;
    using Utils::TGBotApi::JSONKeys::CHAT_ID_KEY;
    using Utils::TGBotApi::JSONKeys::TEXT_KEY;
    using Utils::JSONKeys::USER_ID_KEY;

    const string& MediaCallbackHandler::get_name() const noexcept {
        static const string name = "YouTubeMediaCallbackHandler";
        return name;
    }

    bool MediaCallbackHandler::check(shared_ptr<BotHandlerContext> ctx) {
        return (ctx->access.full || ctx->access.youtube)
            && ctx->callback != nullptr
            && json::accept(ctx->callback->data)
            && json::parse(ctx->callback->data)[0].get<string>() == YOUTUBE_MEDIA_CALLBACK_HANDLER_NAME;
    }

    ptrMessage MediaCallbackHandler::handle(shared_ptr<BotHandlerContext> ctx) {
        ctx->bot->answer_callback_query(ctx->callback->id);
        ctx->bot->delete_message(ctx->chat->id, ctx->message->id);
        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = IN_DEVELOP_PHRASE,
            .reply_message_id = ctx->message->reply_message_id
        });
    }
}