#include <utils/YouTubeApi/YouTubeApi.hpp>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::YouTubeApi {

    using httplib::Result;
    using httplib::Client;
    using httplib::Params;
    using httplib::Error;
    using nlohmann::json;
    using std::runtime_error;
    using std::to_string;
    using std::make_unique;

    #ifndef NDEBUG
    using Logger::get_logger;
    #endif

    YouTubeApi::YouTubeApi(string_view youtube_api_url):
        _youtube_api_url(youtube_api_url)
    {}

    unique_ptr<json> send_request(string_view youtube_api_url, string_view request_url, string_view url) {
        Client clt(youtube_api_url.data());
        Params params{{"url", url.data()}};

        auto result = clt.Get(request_url.data(), params, {});

        #ifndef NDEBUG
        get_logger()->debug("YouTubeApi::send_request::url", url, __FILE__, __LINE__);
        get_logger()->debug("YouTubeApi::send_request::youtube_api_url", youtube_api_url, __FILE__, __LINE__);
        #endif

        if (result.error() != Error::Success) {
            throw runtime_error("YouTubeApi::get_video: " + to_string(result.error()));
        }

        #ifndef NDEBUG
        get_logger()->debug("YouTubeApi::send_request::error", to_string(result.error()), __FILE__, __LINE__);
        get_logger()->debug("YouTubeApi::send_request::status", to_string(result->status), __FILE__, __LINE__);
        get_logger()->debug("YouTubeApi::send_request::body", result->body, __FILE__, __LINE__);
        #endif

        if (result->status != 200 || result->body.empty() || !json::accept(result->body)) {
            return nullptr;
        }

        return make_unique<json>(json::parse(result->body));
    }

    unique_ptr<pair<Video, Channel> > YouTubeApi::get_video(string_view url) {
        auto json_body = send_request(_youtube_api_url, "/video", url);
        if (json_body == nullptr) {
            return nullptr;
        }

        const auto& json_video = json_body->at("video");
        const auto& json_channel = json_body->at("channel");

        return make_unique<pair<Video, Channel> >(pair{
            Video{
                .title = json_video["title"],
                .video_id = json_video["video_id"],
                .video_url = json_video["video_url"],
                .thumbnail_url = json_video["thumbnail_url"],
                .year = json_video["year"]
            },
            Channel{
                .channel_id = json_channel["channel_id"],
                .title = json_channel["title"],
                .channel_url = json_channel["channel_url"],
            }
        });
    }

    unique_ptr<Playlist> YouTubeApi::get_playlist(string_view url) {
        auto json_body = send_request(_youtube_api_url, "/playlist", url);
        if (json_body == nullptr) {
            return nullptr;
        }

        json json_playlist = json_body->at("playlist");

        return make_unique<Playlist>(Playlist{
            .playlist_id = json_playlist["playlist_id"],
            .title = json_playlist["title"],
            .playlist_url = json_playlist["playlist_url"],
            .views = json_playlist["views"],
            .thumbnail_url = json_playlist["thumbnail_url"],
            .video_urls = json_playlist["video_urls"],
        });
    }


}