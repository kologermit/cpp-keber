#pragma once

#include <utils/YouTubeApi/InterfaceYouTubeApi.hpp>

namespace Utils::YouTubeApi {
    struct YouTubeApi final : InterfaceYouTubeApi {
        unique_ptr<pair<Video, Channel> > get_video(string_view url) override;
        unique_ptr<Playlist> get_playlist(string_view url) override;
    };
}