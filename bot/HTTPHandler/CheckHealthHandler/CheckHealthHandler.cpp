#include <bot/HTTPHandler/CheckHealthHandler/CheckHealthHandler.hpp>

namespace Bot::HTTPHandler::CheckHealthHandler {

    const string& CheckHealthHandler::get_name() const noexcept {
        static const string name = "CheckHealthHandler";
        return name;
    }

    RequestHandlerMethod CheckHealthHandler::get_method() const noexcept {
        return GET;
    }

    const string& CheckHealthHandler::get_pattern() const noexcept {
        static const string pattern = "/check_health";
        return pattern;
    }

    json CheckHealthHandler::handle(const Request&, Response&) {
        return "active";
    }
}