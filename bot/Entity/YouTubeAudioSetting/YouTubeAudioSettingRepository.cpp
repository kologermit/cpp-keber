#include <bot/Entity/YouTubeAudioSetting/YouTubeAudioSettingRepository.hpp>

namespace Bot::Entity::YouTubeAudioSetting {

    using Utils::Entity::exec_select_many;
    using Utils::Entity::exec_insert;
    using Utils::Entity::exec_delete_by_ids;

    unique_ptr<vector<YouTubeAudioSetting> > YouTubeAudioSettingRepository::get_by_user_id(long long user_id, bool check_deleted) {
        return exec_select_many<YouTubeAudioSetting>(_db, YouTubeAudioSetting::get_table_name(), {
            {USER_ID_COLUMN, to_string(user_id)}
        }, check_deleted);
    }
    unique_ptr<vector<YouTubeAudioSetting> > YouTubeAudioSettingRepository::update_by_user_id(long long user_id, const vector<YouTubeAudioSetting>& settings) {
        auto old_settings = get_by_user_id(user_id);
        vector<long long> old_setting_ids;
        for (const auto& old_setting : *old_settings) {
            old_setting_ids.push_back(old_setting.id);
        }
        exec_delete_by_ids<YouTubeAudioSetting>(_db, old_setting_ids);
        unique_ptr<vector<YouTubeAudioSetting>> result;
        for (const auto& setting : settings) {
            result->push_back(*create(setting));
        }
        return result;
    }
}