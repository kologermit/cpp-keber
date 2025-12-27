#pragma once

#include <bot/BotHandler/YouTube/Media/MediaCallbackHandler.hpp>
#include <bot/BotHandler/YouTube/Media/MediaHandler.hpp>
#include <bot/BotHandler/YouTube/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <utils/JSONKeys.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <fmt/core.h>

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

    bool MediaCallbackHandler::check(shared_ptr<BotHandlerContext> context) {
        return (context->access.full || context->access.youtube)
            && context->callback != nullptr
            && json::accept(context->callback->data)
            && json::parse(context->callback->data)[0].get<string>() == YOUTUBE_MEDIA_CALLBACK_HANDLER_NAME;
    }

    ptrMessage MediaCallbackHandler::handle(shared_ptr<BotHandlerContext> context) {
        auto queue = get_downloader_queue(context->config);
        context->bot->answer_callback_query(context->callback->id);
        context->bot->delete_message(context->chat->id, context->message->id);
        return context->bot->send_message({
            .chat_id = context->chat->id,
            .text = (
                queue->publish_message({
                    {DATA_KEY, json::parse(context->callback->data)},
                    {TEXT_KEY, context->message->text},
                    {CHAT_ID_KEY, context->chat->id},
                    {USER_ID_KEY, context->user->id},
                })
                ? ADD_TO_QUEUE_PHRASE
                : FAILED_TO_ADD_TO_QUEUE_PHRASE
            ),
            .reply_message_id = context->message->reply_message_id
        });
    }
}