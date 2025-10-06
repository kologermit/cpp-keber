#pragma once

#include <vector>
#include <utils/Entity/Repository.hpp>
#include <bot/Entity/YouTubeAudioSetting/YouTubeAudioSetting.hpp>

namespace Bot::Entity::YouTubeAudioSetting {

    using Utils::Entity::Repository;
    using pqxx::connection;
    using std::vector;
    using std::unique_ptr;

    struct InterfaceYouTubeAudioSettingRepository : Repository<YouTubeAudioSetting> {
        InterfaceYouTubeAudioSettingRepository(connection& db): Repository(db) {}
        virtual vector<unique_ptr<YouTubeAudioSetting> > get_by_user_id(long long user_id, bool check_deleted = true) = 0;
        virtual vector<unique_ptr<YouTubeAudioSetting> > update_by_user_id(long long user_id, vector<unique_ptr<YouTubeAudioSetting> > settings) = 0;
    };

}