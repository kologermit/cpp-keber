#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::YouTube::Media {

    constexpr const char* YOUTUBE_MEDIA_CALLBACK_HANDLER_NAME = "ymch";

    struct MediaHandler final : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> context) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> context) override;

        ptrMessage handle_urls(shared_ptr<BotHandlerContext> context);
        ptrMessage send_audio_settings(shared_ptr<BotHandlerContext> context);
        ptrMessage handle_audio_settings(shared_ptr<BotHandlerContext> context);

        static ptrMessage to_youtube_media(shared_ptr<BotHandlerContext> context, bool is_video);
    };
}