#pragma once

#include <utils/HTTPServer/Handler/InterfaceHandler.hpp>
#include <bot/HTTPHandler/HandlerContext.hpp>

namespace Bot::HTTPHandler::Health {

    using nlohmann::json;
    using Utils::HTTPServer::Handler::InterfaceHandler;
    using Utils::HTTPServer::Handler::HandlerSignature;

    struct GETHealthHandler : InterfaceHandler<HandlerContext> {
        const HandlerSignature& get_signature() const noexcept override;
        json handle(ptrContext context) override;
    };

}