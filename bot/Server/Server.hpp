#pragma once

#include <bot/Server/InterfaceServer.hpp>
#include <bot/Entity/Repositories.hpp>
#include <bot/BotHandler/InterfaceBotHandler.hpp>
#include <bot/HTTPHandler/InterfaceHTTPHandler.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <vector>

namespace Bot::Server {

using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using HTTPServer = httplib::Server;
using Bot::Entity::Repositories::Repositories;
using Bot::BotHandler::InterfaceBotHandler;
using Bot::HTTPHandler::InterfaceHTTPHandler;
using Utils::TGBotApi::Bot::InterfaceBot;

struct Server : InterfaceServer {
    void stop() override;
    void run() noexcept override;

    Server(
        vector<unique_ptr<InterfaceBotHandler> >&& bot_handlers,
        vector<unique_ptr<InterfaceHTTPHandler> >&& request_handlers
    );

    Server();

    private:

    vector<unique_ptr<InterfaceBotHandler> > bot_handlers;
    vector<unique_ptr<InterfaceHTTPHandler> > request_handlers;
    HTTPServer _server;

};

}