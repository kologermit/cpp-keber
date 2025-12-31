#include <bot/HTTPHandler/Health/GETHealthHandler.hpp>

namespace Bot::HTTPHandler::Health {
    using Utils::HTTPServer::Handler::RequestHandlerMethod;

    const HandlerSignature& GETHealthHandler::get_signature() const noexcept {
        static const HandlerSignature signature{
            .name = "GETHealthHandler",
            .pattern = "/health",
            .method = RequestHandlerMethod::GET,
        };
        return signature;
    }

    json GETHealthHandler::handle(ptrContext) {
        return "active";
    }
}