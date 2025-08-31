#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::YouTubeHandler::PlaylistHandler {

    constexpr const char* YOUTUBE_PLAYLIST_CALLBACK_HANDLER_NAME = "ypch";

    struct YouTubePlaylistHandler final : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> context) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> context) override;

        static ptrMessage to_youtube_playlist(shared_ptr<BotHandlerContext> context, bool is_video);
    };
}