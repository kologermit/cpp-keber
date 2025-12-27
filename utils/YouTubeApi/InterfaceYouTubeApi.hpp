#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Utils::YouTubeApi {

    using std::string;
    using std::string_view;
    using std::unique_ptr;
    using std::shared_ptr;
    using std::vector;
    using std::pair;

    struct Video {
        const string title;
        const string video_id;
        const string video_url;
        const string thumbnail_url;
        const int year;
    };

    struct Channel {
        const string channel_id;
        const string title;
        const string channel_url;
    };

    struct Playlist {
        const string playlist_id;
        const string title;
        const string playlist_url;
        const int views;
        const string thumbnail_url;
        const vector<string> video_urls;
    };

    struct InterfaceYouTubeApi {
        virtual ~InterfaceYouTubeApi() = default;

        virtual unique_ptr<pair<Video, Channel> > get_video(string_view url) = 0;
        virtual unique_ptr<Playlist> get_playlist(string_view url) = 0;
    };
}