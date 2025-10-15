#pragma once

#include <bot/HTTPHandler/InterfaceHTTPHandler.hpp>

namespace Bot::HTTPHandler::SendMessageHandler {
    struct SendMessageHandler final : InterfaceHTTPHandler {
        const string& get_name() const noexcept override;
        RequestHandlerMethod get_method() const noexcept override;
        const string& get_pattern() const noexcept override;
        json handle(const Request& req, Response& res) override;
    };
}