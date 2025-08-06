#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/TGBotApi/Chat/Chat.hpp>
#include <string>
#include <string_view>

namespace Bot::Entity {

using Utils::Entity::Entity;
using std::string;
using std::string_view;
using std::optional;
using std::nullopt;
using jed_utils::datetime;

enum EnumUserScreen {
    UNKNOWN,
    START,

};

struct User : Entity {

    long long telegram_id;
    string name;
    string username;
    EnumUserScreen screen;

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

};

}