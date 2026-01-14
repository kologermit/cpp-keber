#pragma once

#include <utils/Api/Entity.hpp>

namespace Utils::BotApi {
    using std::string;
    using nlohmann::json;
    using Utils::Api::Entity;

    constexpr const char* NAME_KEY = "name";
    constexpr const char* USERNAME_KEY = "username";
    constexpr const char* SCREEN_KEY = "screen";

    enum UserScreen : int {
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

    struct User : Entity {
        string name;
        string username;
        UserScreen screen;

        User(const json& json_user):
            Entity(json_user),
            name(json_user[NAME_KEY].get<string>()),
            username(json_user[USERNAME_KEY].get<string>()),
            screen(static_cast<UserScreen>(json_user[SCREEN_KEY].get<int>()))
        {}
    };
}