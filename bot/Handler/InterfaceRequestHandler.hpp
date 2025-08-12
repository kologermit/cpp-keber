#pragma once

#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Query/Query.hpp>

namespace Bot::Handler {

using Bot::Entity::Repositories::Repositories;
using Utils::TGBotApi::Query::EnumQueryMethod;
using Handler = httplib::Server::Handler;
using std::string;
using std::shared_ptr;

struct InterfaceRequestHandler {

    virtual EnumQueryMethod get_method() const noexcept = 0;
    virtual const string& get_pattern() const noexcept = 0;
    virtual void call() = 0;

};

}