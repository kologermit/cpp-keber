#include <task_tracker/HTTPHandler/Health/GETHealthHandler.hpp>

namespace TaskTracker::HTTPHandler::Health {
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