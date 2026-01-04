#include <bot/HTTPHandler/init.hpp>
#include <bot/HTTPHandler/Health/GETHealthHandler.hpp>
#include <bot/HTTPHandler/Webhook/WebhookHandler.hpp>
#include <bot/HTTPHandler/Message/POSTMessageHandler.hpp>
#include <bot/HTTPHandler/Message/PATCHMessageHandler.hpp>
#include <bot/HTTPHandler/User/GETUserHandler.hpp>

namespace Bot::HTTPHandler {
    using std::vector;
    using std::make_shared;
    using Bot::HTTPHandler::Health::GETHealthHandler;
    using Bot::HTTPHandler::Webhook::WebhookHandler;
    using Bot::HTTPHandler::Message::POSTMessageHandler;
    using Bot::HTTPHandler::Message::PATCHMessageHandler;
    using Bot::HTTPHandler::User::GETUserHandler;

    void init_server(IServer& server) {
        server.add_handlers(vector<IServer::ptrIHandler>{
            make_shared<GETHealthHandler>(),
            make_shared<WebhookHandler>(),
            make_shared<POSTMessageHandler>(),
            make_shared<PATCHMessageHandler>(),
            make_shared<GETUserHandler>(),
        });
    }
}