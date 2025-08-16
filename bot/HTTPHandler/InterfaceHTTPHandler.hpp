#pragma once

#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Query/Query.hpp>
#include <nlohmann/json.hpp>

namespace Bot::HTTPHandler {

using Bot::Entity::Repositories::Repositories;
using Handler = httplib::Server::Handler;
using httplib::Request;
using httplib::Response;
using nlohmann::json;
using std::pair;
using std::string;
using std::shared_ptr;

enum RequestHandlerMethod {
    GET,
    POST,
    PATCH,
    DELETE
};

struct InterfaceHTTPHandler {

    virtual RequestHandlerMethod get_method() const noexcept = 0;
    virtual const string& get_pattern() const noexcept = 0;
    virtual json handle(const Request& req, Response& res) = 0;

};

}