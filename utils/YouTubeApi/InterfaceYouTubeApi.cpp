#include <utils/YouTubeApi/InterfaceYouTubeApi.hpp>
#include <stdexcept>

namespace Utils::YouTubeApi {

    using std::invalid_argument;
    using std::move;

    shared_ptr<InterfaceYouTubeApi> get_youtube_api(unique_ptr<InterfaceYouTubeApi> youtube_api) {
        static shared_ptr<InterfaceYouTubeApi> _youtube_api;

        if (_youtube_api == nullptr && youtube_api == nullptr) {
            throw invalid_argument("get_youtube_api is not initialized!");
        }

        if (_youtube_api == nullptr) {
            _youtube_api = move(youtube_api);
        }

        return _youtube_api;
    }
}