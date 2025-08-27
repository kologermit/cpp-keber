#pragma once

#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>

namespace Bot::BotHandler {

using Bot::Entity::Repositories::Repositories;
using Bot::Entity::Message::Message;
using Bot::Entity::Chat::Chat;
using Bot::Entity::User::User;
using Bot::Entity::Access::UserAccess;
using Utils::TGBotApi::Bot::InterfaceBot;
using Utils::TGBotApi::Types::ptrMessage;
using std::shared_ptr;
using std::string;
using std::vector;

struct BotHandlerContext {
    
    shared_ptr<Message> message;
    shared_ptr<Chat> chat;
    shared_ptr<User> user;
    UserAccess access;

};

struct InterfaceBotHandler {

    virtual const string& get_name() const noexcept = 0;
    virtual bool check(shared_ptr<BotHandlerContext> context) = 0;
    virtual ptrMessage handle(shared_ptr<BotHandlerContext> context) = 0;

};

}