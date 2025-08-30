#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::YouTubeHandler {

    enum EnumDownloadType {
        AUDIO,
        VIDEO_720P, VIDEO_BEST,
        PLAYLIST_AUDIO,
        PLAYLIST_VIDEO_720P, PLAYLIST_VIDEO_BEST,
    };

    struct YouTubeHandler final : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> context) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> context) override;

        static ptrMessage to_youtube(shared_ptr<BotHandlerContext> context);
    };

}