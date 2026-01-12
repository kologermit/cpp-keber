#pragma once

#include <utils/Entity/Entity.hpp>

namespace Bot::Entity::YouTubeAudioSetting {

    using Utils::Entity::Entity;
    using jed_utils::datetime;
    using pqxx::row;
    using std::map;
    using std::string;
    using std::string_view;
    using std::optional;
    using std::nullopt;
    using std::to_string;

    const auto USER_ID_COLUMN = "user_id";
    const auto URL_COLUMN = "url";
    const auto FILE_NAME_COLUMN = "file_name";
    const auto DOWNLOAD_URL_COLUMN = "download_url";

    struct YouTubeAudioSetting : Entity {
        long long user_id;
        string url;
        string file_name;
        string download_url;

        YouTubeAudioSetting(
            long long user_id = 0,
            string_view url = "",
            string_view file_name = "",
            string_view download_url = "",
            long long id = 0,
            datetime created_at = {},
            datetime updated_at = {},
            optional<datetime> deleted_at = nullopt
        ):
            Entity(id, created_at, updated_at, deleted_at),
            user_id(user_id),
            url(url),
            file_name(file_name),
            download_url(download_url)
        {}

        YouTubeAudioSetting(const row& youtube_audio_setting_row):
            Entity(youtube_audio_setting_row),
            user_id(youtube_audio_setting_row[USER_ID_COLUMN].get<long long>().value()),
            url(youtube_audio_setting_row[URL_COLUMN].get<string>().value()),
            file_name(youtube_audio_setting_row[FILE_NAME_COLUMN].get<string>().value()),
            download_url(youtube_audio_setting_row[DOWNLOAD_URL_COLUMN].get<string>().value())
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "youtube_audio_settings";
            return table;
        }

        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);
            
            result[USER_ID_COLUMN] = to_string(user_id);
            result[URL_COLUMN] = url;
            result[FILE_NAME_COLUMN] = file_name;
            result[DOWNLOAD_URL_COLUMN] = download_url;

            return result;
        }
    };
}