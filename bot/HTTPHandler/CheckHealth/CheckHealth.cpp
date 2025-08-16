#include <bot/HTTPHandler/CheckHealth/CheckHealth.hpp>

namespace Bot::HTTPHandler::CheckHealth {

RequestHandlerMethod CheckHealth::get_method() const noexcept {
    return GET;
}

const string& CheckHealth::get_pattern() const noexcept {
    static const string pattern = "/check_health";
    return pattern;
}

json CheckHealth::handle(const Request&, Response&) {
    return {"active"};
}

}