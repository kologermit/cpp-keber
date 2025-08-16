#pragma once

#include <bot/Entity/Chat/Chat.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <memory>
#include <vector>

namespace Bot::Entity::Chat {

using std::unique_ptr;
using std::vector;
using TGChat = Utils::TGBotApi::Types::Chat;

struct InterfaceChatRepository {
    virtual unique_ptr<Chat> get_by_id(int id) = 0;
    virtual unique_ptr<Chat> get_by_telegram_id(long long telegram_id) = 0;
    virtual unique_ptr<Chat> get_by_telegram_chat(const TGChat& tg_user) = 0;
    virtual unique_ptr<Chat> create(const Chat& chat) = 0;
    virtual unique_ptr<Chat> update(const Chat& chat, vector<int> columns) = 0;
    virtual unique_ptr<Chat> del(int id) = 0;
};

}