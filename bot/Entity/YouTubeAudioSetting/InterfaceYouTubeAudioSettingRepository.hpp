#pragma once

#include <utils/Entity/Repository.hpp>
#include <bot/Entity/YouTubeAudioSetting/YouTubeAudioSetting.hpp>

namespace Bot::Entity::YouTubeAudioSetting {

    using Utils::Entity::Repository;
    using pqxx::connection;
    using std::vector;
    using std::unique_ptr;

    struct InterfaceYouTubeAudioSettingRepository : Repository<YouTubeAudioSetting> {
        explicit InterfaceYouTubeAudioSettingRepository(connection& db): Repository(db) {}
        virtual unique_ptr<vector<YouTubeAudioSetting> > get_by_user_id(long long user_id, bool check_deleted = true) = 0;
        virtual unique_ptr<vector<YouTubeAudioSetting> > update_by_user_id(long long user_id, const vector<YouTubeAudioSetting>& settings) = 0;
    };

}