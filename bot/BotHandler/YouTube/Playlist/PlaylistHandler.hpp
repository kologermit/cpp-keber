#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>

namespace Bot::BotHandler::YouTube::Playlist {

    constexpr const char* YOUTUBE_PLAYLIST_CALLBACK_HANDLER_NAME = "ypch";

    struct PlaylistHandler final : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> ctx) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> ctx) override;

        static ptrMessage to_youtube_playlist(shared_ptr<BotHandlerContext> ctx, bool is_video);
    };
}