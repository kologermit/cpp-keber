#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>
#include <bot/Config/InterfaceConfig.hpp>

namespace Bot::BotHandler::YouTube {

    using std::shared_ptr;
    using std::make_shared;
    using Bot::Config::InterfaceConfig;

    enum DownloadType {
        AUDIO,
        VIDEO_720P, VIDEO_BEST,
        PLAYLIST_AUDIO,
        PLAYLIST_VIDEO_720P, PLAYLIST_VIDEO_BEST,
    };

    struct YouTubeHandler final : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> ctx) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> ctx) override;

        static ptrMessage to_youtube(shared_ptr<BotHandlerContext> ctx);
    };
}