#pragma once

#include <utils/YouTubeApi/InterfaceYouTubeApi.hpp>
#include <string_view>
#include <string>

namespace Utils::YouTubeApi {
    using std::string_view;
    using std::string;
    struct YouTubeApi final : InterfaceYouTubeApi {
        YouTubeApi(string_view youtube_api_url);
        unique_ptr<pair<Video, Channel> > get_video(string_view url) override;
        unique_ptr<Playlist> get_playlist(string_view url) override;

        private:
        const string _youtube_api_url;
    };
}