#pragma once

#include <utils/Entity/ApiRequest/InterfaceApiRequestRepository.hpp>
#include <bot/Entity/Message/InterfaceMessageRepository.hpp>
#include <bot/Entity/User/InterfaceUserRepository.hpp>
#include <bot/Entity/Chat/InterfaceChatRepository.hpp>
#include <pqxx/pqxx>
#include <stdexcept>

namespace Bot::Entity::Repositories {

using Utils::Entity::ApiRequest::InterfaceApiRequestRepository;
using Bot::Entity::Message::InterfaceMessageRepository;
using Bot::Entity::User::InterfaceUserRepository;
using Bot::Entity::Chat::InterfaceChatRepository;
using pqxx::connection;
using std::shared_ptr;
using std::unique_ptr;
using std::runtime_error;
using std::string;
using std::string_view;
using std::move;

class Repositories {

    connection _db;

    public:

    Repositories(string_view conn_str);

    shared_ptr<InterfaceApiRequestRepository> api_request_repository;
    shared_ptr<InterfaceMessageRepository> message_repository;
    shared_ptr<InterfaceUserRepository> user_repository;
    shared_ptr<InterfaceChatRepository> chat_repository;

};

shared_ptr<Repositories> get_repositories(unique_ptr<Repositories> repositories = nullptr);

}