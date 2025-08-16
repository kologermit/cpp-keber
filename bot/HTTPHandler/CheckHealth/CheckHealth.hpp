#pragma once

#include <bot/HTTPHandler/InterfaceHTTPHandler.hpp>

namespace Bot::HTTPHandler::CheckHealth {

struct CheckHealth : InterfaceHTTPHandler {

    RequestHandlerMethod get_method() const noexcept override;
    const string& get_pattern() const noexcept override;
    json handle(const Request& req, Response& res) override;

};

}