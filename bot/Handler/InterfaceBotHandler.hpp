#pragma once

#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>

namespace Bot::Handler {

using Bot::Entity::Repositories::Repositories;
using Bot::Entity::Message::Message;
using Bot::Entity::Chat::Chat;
using Bot::Entity::User::User;
using Utils::TGBotApi::Bot::InterfaceBot;
using Utils::TGBotApi::Types::ptrMessage;
using std::shared_ptr;

struct BotHandlerContext {

    shared_ptr<Repositories> repositories;
    shared_ptr<InterfaceBot> bot;
    Message& message;
    Chat& chat;
    User& user;

};

struct InterfaceBotHandler {

    virtual bool check(shared_ptr<BotHandlerContext> context) = 0;
    virtual ptrMessage handle(shared_ptr<BotHandlerContext> context) = 0;

};

}