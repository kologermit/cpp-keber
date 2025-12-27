#include <utils/HTTPServer/Handler/NotFoundHandler.hpp>
#include <bot/HTTPHandler/init.hpp>
#include <bot/HTTPHandler/CheckHealth/CheckHealthHandler.hpp>
#include <bot/HTTPHandler/Webhook/WebhookHandler.hpp>

namespace Bot::HTTPHandler {
    using std::vector;
    using std::make_shared;
    using Utils::HTTPServer::Handler::NotFoundHandler;
    using Bot::HTTPHandler::CheckHealth::CheckHealthHandler;
    using Bot::HTTPHandler::Webhook::WebhookHandler;

    void init_server(IServer& server) {
        server.add_handlers(vector<IServer::ptrIHandler>{
            make_shared<CheckHealthHandler>(),
            make_shared<WebhookHandler>(),
            make_shared<NotFoundHandler<HandlerContext> >(),
        });
    }
}