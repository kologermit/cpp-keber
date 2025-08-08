#pragma once

#include <bot/Entity/User/User.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <memory>
#include <vector>

namespace Bot::Entity::User {
    
using std::unique_ptr;
using std::vector;
using TGUser = Utils::TGBotApi::Types::User;

struct InterfaceUserRepository {

    virtual unique_ptr<User> get_by_id(int id) = 0;
    virtual unique_ptr<User> get_by_telegram_id(long long telegram_id) = 0;
    virtual unique_ptr<User> get_by_telegram_user(const TGUser& tg_user) = 0;
    virtual unique_ptr<User> create(const User& user) = 0;
    virtual unique_ptr<User> update(const User& user, vector<int> columns) = 0;
    virtual unique_ptr<User> del(int id) = 0;

};

}