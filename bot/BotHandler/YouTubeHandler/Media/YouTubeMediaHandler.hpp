#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::YouTubeHandler::Media {

    constexpr const char* YOUTUBE_MEDIA_CALLBACK_HANDLER_NAME = "ymch";

    struct YouTubeMediaHandler final : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> context) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> context) override;

        static ptrMessage to_youtube_media(shared_ptr<BotHandlerContext> context, bool is_video);
    };
}