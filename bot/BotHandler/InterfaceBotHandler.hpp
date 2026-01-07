#pragma once

#include <bot/GlobalContext.hpp>
#include <bot/HTTPHandler/HandlerContext.hpp>
#include <utils/TGBotApi/Types.hpp>

namespace Bot::BotHandler {

    using Bot::Entity::Message::Message;
    using Bot::Entity::Chat::Chat;
    using Bot::Entity::User::User;
    using Bot::Entity::Callback::Callback;
    using Bot::Entity::Access::UserAccess;
    using Bot::HTTPHandler::HandlerContext;
    using Bot::Config::InterfaceConfig;
    using Utils::TGBotApi::Bot::InterfaceBot;
    using Utils::TGBotApi::Types::ptrMessage;
    using std::shared_ptr;
    using std::string;
    using std::vector;

    struct BotHandlerContext {
        shared_ptr<Message> message;
        shared_ptr<Callback> callback;
        shared_ptr<Chat> chat;
        shared_ptr<User> user;
        shared_ptr<User> bot_user;
        shared_ptr<GlobalContext> global_ctx;
        shared_ptr<HandlerContext> handler_ctx;
        shared_ptr<DBContext> db;
        UserAccess access;
        shared_ptr<InterfaceConfig> config;
        shared_ptr<InterfaceBot> bot;
    };

    struct InterfaceBotHandler {
        using ptrContext = shared_ptr<BotHandlerContext>;

        virtual const string& get_name() const noexcept = 0;
        virtual bool check(ptrContext ctx) = 0;
        virtual ptrMessage handle(ptrContext ctx) = 0;

        virtual ~InterfaceBotHandler() = default;

    };

}