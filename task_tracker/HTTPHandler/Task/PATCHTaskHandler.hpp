#pragma once

#include <utils/HTTPServer/Handler/InterfaceHandler.hpp>
#include <task_tracker/HTTPHandler/HandlerContext.hpp>

namespace TaskTracker::HTTPHandler::Task {

    using nlohmann::json;
    using Utils::HTTPServer::Handler::InterfaceHandler;
    using Utils::HTTPServer::Handler::HandlerSignature;

    struct PATCHTaskHandler : InterfaceHandler<HandlerContext> {
        const HandlerSignature& get_signature() const noexcept override;
        json handle(ptrContext ctx) override;
    };
}