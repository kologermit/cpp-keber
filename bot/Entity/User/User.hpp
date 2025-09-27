#pragma once

#include <utils/Entity/Entity.hpp>
#include <string>
#include <map>
#include <string_view>
#include <pqxx/pqxx>

namespace Bot::Entity::User {

    using Utils::Entity::Entity;
    using Utils::Entity::Column;
    using Utils::Entity::NAME;
    using std::string;
    using std::string_view;
    using std::optional;
    using std::to_string;
    using std::nullopt;
    using std::map;
    using std::make_shared;
    using pqxx::row;
    using jed_utils::datetime;

    enum EnumUserScreen : int {
        UNKNOWN,
        START,
        MENU,
        DOCKER,
        DICE,
        ACCESS, ADD_ACCESS,
        WEATHER, ADD_WEATHER_POINT,
        TASK, ADD_TASK_DESCRIPTION, ADD_TASK_DATE, EDIT_TASK_SUMMARY, EDIT_TASK_DESCRIPTION, EDIT_TASK_DATE,
        YOUTUBE, YOUTUBE_VIDEO, YOUTUBE_AUDIO, YOUTUBE_PLAYLIST_VIDEO, YOUTUBE_PLAYLIST_AUDIO,
    };
    constexpr const char* USER_SCREENS_TABLE = "user_screens";
    const map<int, string> map_user_screen_to_string{
        {UNKNOWN, "UNKNOWN"},
        {MENU, "MENU"},
        {DICE, "DICE"},
        {ACCESS, "ACCESS"},
        {ADD_ACCESS, "ADD_ACCESS"},
        {WEATHER, "WEATHER"},
        {ADD_WEATHER_POINT, "ADD_WEATHER_POINT"},
        {TASK, "TASK"},
        {ADD_TASK_DESCRIPTION, "ADD_TASK_DESCRIPTION"},
        {ADD_TASK_DATE, "ADD_TASK_DATE"},
        {EDIT_TASK_SUMMARY, "EDIT_TASK_SUMMARY"},
        {EDIT_TASK_DESCRIPTION, "EDIT_TASK_DESCRIPTION"},
        {YOUTUBE, "YOUTUBE"},
        {YOUTUBE_VIDEO, "YOUTUBE_VIDEO"},
        {YOUTUBE_AUDIO, "YOUTUBE_AUDIO"},
        {YOUTUBE_PLAYLIST_VIDEO, "YOUTUBE_PLAYLIST_VIDEO"},
        {YOUTUBE_PLAYLIST_AUDIO, "YOUTUBE_PLAYLIST_AUDIO"},
    };

    const auto USERNAME = make_shared<Column>("username");
    const auto SCREEN = make_shared<Column>("screen");

    struct User : Entity {

        string name;
        string username;
        EnumUserScreen screen;

        User(
            string_view name = "",
            string_view username = "",
            EnumUserScreen screen = UNKNOWN,
            long long id = 0,
            datetime created_at = {},
            datetime updated_at = {},
            optional<datetime> deleted_at = nullopt
        ):
            Entity(id, created_at, updated_at, deleted_at),
            name(name),
            username(username),
            screen(screen)
        {}

        User(const row& user_row):
            Entity(user_row),
            name(user_row[NAME->name].get<string>().value()),
            username(user_row[USERNAME->name].get<string>().value()),
            screen(static_cast<EnumUserScreen>(user_row[SCREEN->name].get<int>().value()))
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "users";
            return table;
        }

        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);
            
            result[NAME->name] = name;
            result[USERNAME->name] = username;
            result[SCREEN->name] = to_string(screen);

            return result;
        }
    };
}