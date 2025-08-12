#pragma once

#include <bot/Server/InterfaceServer.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <vector>

namespace Bot::Server {

using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using HTTPServer = httplib::Server;
using Bot::Entity::Repositories::Repositories;
using Utils::TGBotApi::Bot::InterfaceBot;

struct Server : InterfaceServer {

    void add_bot_handler(unique_ptr<InterfaceBotHandler> handler) noexcept override;
    void add_request_handler(unique_ptr<InterfaceRequestHandler> handler) noexcept override;
    void run() noexcept override;

    Server(shared_ptr<Repositories> repositories, shared_ptr<InterfaceBot> bot);

    private:

    vector<unique_ptr<InterfaceBotHandler> > _bot_handlers;
    vector<unique_ptr<InterfaceRequestHandler> > _request_handlers;

    shared_ptr<Repositories> _repositories;
    shared_ptr<InterfaceBot> _bot;

    HTTPServer _server;

};

}