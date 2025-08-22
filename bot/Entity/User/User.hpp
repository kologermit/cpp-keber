#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/TGBotApi/Chat/Chat.hpp>
#include <string>
#include <map>
#include <string_view>
#include <pqxx/pqxx>

namespace Bot::Entity::User {

using Utils::Entity::Entity;
using std::string;
using std::string_view;
using std::optional;
using std::to_string;
using std::nullopt;
using std::map;
using pqxx::row;
using jed_utils::datetime;

enum EnumUserScreen {
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

const map<int, string> map_user_screen_to_string{
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(UNKNOWN),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(START),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(MENU),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(DICE),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(ACCESS),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(ADD_ACCESS),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(WEATHER),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(ADD_WEATHER_POINT),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(TASK),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(ADD_TASK_DESCRIPTION),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(ADD_TASK_DATE),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EDIT_TASK_SUMMARY),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EDIT_TASK_DESCRIPTION),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(EDIT_TASK_DATE),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(YOUTUBE),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(YOUTUBE_VIDEO),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(YOUTUBE_AUDIO),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(YOUTUBE_PLAYLIST_VIDEO),
    GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(YOUTUBE_PLAYLIST_AUDIO),
};

enum UserColumns {
    TELEGRAM_ID,
    NAME,
    USERNAME,
    SCREEN,
};

constexpr const char* USERS_TABLE = "\"users\"";
constexpr const char* USER_SCREENS_TABLE = "\"user_screens\"";
constexpr const char* TELEGRAM_ID_COLUMN = "\"telegram_id\"";
constexpr const char* NAME_COLUMN = "\"name\"";
constexpr const char* USERNAME_COLUMN = "\"username\"";
constexpr const char* SCREEN_COLUMN = "\"screen\"";

struct User : Entity {

    long long telegram_id;
    string name;
    string username;
    EnumUserScreen screen;

    User(
        long long telegram_id = 0,
        string_view name = "",
        string_view username = "",
        EnumUserScreen screen = EnumUserScreen::UNKNOWN
    ):
        Entity(0, nullopt, nullopt, nullopt),
        telegram_id(telegram_id),
        name(name),
        username(username),
        screen(screen)
    {}

    User(
        int id,
        optional<datetime> created_at = nullopt,
        optional<datetime> updated_at = nullopt,
        optional<datetime> deleted_at = nullopt,
        long long telegram_id = 0,
        string_view name = "",
        string_view username = "",
        EnumUserScreen screen = EnumUserScreen::UNKNOWN
    ):
        Entity(id, created_at, updated_at, deleted_at),
        telegram_id(telegram_id),
        name(name),
        username(username),
        screen(screen)
    {}

    User(const row& user_row):
        Entity(user_row),
        telegram_id(user_row[TELEGRAM_ID_COLUMN].get<long long>().value()),
        name(user_row[NAME_COLUMN].get<string>().value()),
        username(user_row[USERNAME_COLUMN].get<string>().value()),
        screen(static_cast<EnumUserScreen>(user_row[SCREEN_COLUMN].get<int>().value()))
    {}

    string get_data_by_column(int column) const {
        switch (column)
        {
        case TELEGRAM_ID:
            return to_string(telegram_id);
            break;

        case NAME:
            return name;
            break; 

        case USERNAME:
            return username;
            break;

        case SCREEN:
            return to_string(screen);
            break;
        
        default:
            return "";
            break;
        }
    }

};

}