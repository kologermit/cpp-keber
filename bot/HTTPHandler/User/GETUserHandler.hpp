#pragma once

#include <utils/HTTPServer/Handler/InterfaceHandler.hpp>
#include <bot/HTTPHandler/HandlerContext.hpp>

namespace Bot::HTTPHandler::User {

    using Utils::HTTPServer::Handler::InterfaceHandler;
    using Utils::HTTPServer::Handler::HandlerSignature;

    struct GETUserHandler : InterfaceHandler<HandlerContext> {
        const HandlerSignature &get_signature() const noexcept override;
        json handle(ptrContext context) override;
    };
}