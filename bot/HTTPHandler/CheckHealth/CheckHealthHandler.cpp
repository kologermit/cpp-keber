#include <bot/HTTPHandler/CheckHealth/CheckHealthHandler.hpp>

namespace Bot::HTTPHandler::CheckHealth {
    using Utils::HTTPServer::Handler::RequestHandlerMethod;

    const HandlerSignature& CheckHealthHandler::get_signature() const noexcept {
        static const HandlerSignature signature{
            .name = "CheckHealthHandler",
            .pattern = "/check_health",
            .method = RequestHandlerMethod::GET,
        };
        return signature;
    }

    json CheckHealthHandler::handle(ptrContext) {
        return "active";
    }
}