#pragma once

#include <bot/BotHandler/YouTubeHandler/MediaHandler/YouTubeMediaCallbackHandler.hpp>
#include <bot/BotHandler/YouTubeHandler/MediaHandler/YouTubeMediaHandler.hpp>
#include <bot/BotHandler/YouTubeHandler/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <fmt/core.h>

namespace Bot::BotHandler::YouTubeHandler::MediaHandler {

    using nlohmann::json;
    using Utils::TGBotApi::Bot::get_bot;
    using Utils::TGBotApi::JSONKeys::DATA_KEY;
    using Utils::TGBotApi::JSONKeys::CALLBACK_DATA_KEY;
    using Utils::TGBotApi::JSONKeys::CALLBACK_QUERY_ID_KEY;
    using Utils::TGBotApi::JSONKeys::CHAT_ID_KEY;
    using Utils::TGBotApi::JSONKeys::TEXT_KEY;

    const string& YouTubeMediaCallbackHandler::get_name() const noexcept {
        static const string name = "YouTubeMediaCallbackHandler";
        return name;
    }

    bool YouTubeMediaCallbackHandler::check(shared_ptr<BotHandlerContext> context) {
        return (context->access.full || context->access.youtube)
            && context->callback != nullptr
            && json::accept(context->callback->data)
            && json::parse(context->callback->data)[0].get<string>() == YOUTUBE_MEDIA_CALLBACK_HANDLER_NAME;
    }

    ptrMessage YouTubeMediaCallbackHandler::handle(shared_ptr<BotHandlerContext> context) {
        auto queue = get_downloader_queue();
        get_bot()->answer_callback_query(context->callback->id);
        get_bot()->delete_message(context->chat->id, context->message->id);
        return get_bot()->send_message({
            .chat_id = context->chat->id,
            .text = (
                queue->publish_message({
                    {DATA_KEY, json::parse(context->callback->data)},
                    {TEXT_KEY, context->message->text},
                    {CHAT_ID_KEY, context->chat->id},
                })
                ? ADD_TO_QUEUE_PHRASE
                : FAILED_TO_ADD_TO_QUEUE_PHRASE
            ),
            .reply_message_id = context->message->reply_message_id
        });
    }
}