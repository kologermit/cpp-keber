#pragma once

#include <bot/Entity/YouTubeAudioSetting/InterfaceYouTubeAudioSettingRepository.hpp>

namespace Bot::Entity::YouTubeAudioSetting {
    struct YouTubeAudioSettingRepository final : InterfaceYouTubeAudioSettingRepository {
        explicit YouTubeAudioSettingRepository(connection& db): InterfaceYouTubeAudioSettingRepository(db) {}
        unique_ptr<vector<YouTubeAudioSetting> > get_by_user_id(long long user_id, bool check_deleted = true) override;
        unique_ptr<vector<YouTubeAudioSetting> > update_by_user_id(long long user_id, const vector<YouTubeAudioSetting>& settings) override;
    };
}