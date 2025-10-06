#pragma once

#include <bot/Entity/YouTubeAudioSetting/InterfaceYouTubeAudioSettingRepository.hpp>

namespace Bot::Entity::YouTubeAudioSetting {
    struct YouTubeAudioSettingRepository final : InterfaceYouTubeAudioSettingRepository {
        YouTubeAudioSettingRepository(connection& db): InterfaceYouTubeAudioSettingRepository(db) {}
        vector<unique_ptr<YouTubeAudioSetting> > get_by_user_id(long long user_id, bool check_deleted = true) override;
        vector<unique_ptr<YouTubeAudioSetting> > update_by_user_id(long long user_id, vector<unique_ptr<YouTubeAudioSetting> > settings) override;
    };
}