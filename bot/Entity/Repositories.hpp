#pragma once

#include <utils/Entity/ApiRequest/InterfaceApiRequestRepository.hpp>
#include <bot/Entity/Message/InterfaceMessageRepository.hpp>
#include <bot/Entity/User/InterfaceUserRepository.hpp>
#include <bot/Entity/Chat/InterfaceChatRepository.hpp>
#include <bot/Entity/Access/InterfaceAccessRepository.hpp>
#include <bot/Entity/Callback/InterfaceCallbackRepository.hpp>
#include <pqxx/pqxx>
#include <stdexcept>

namespace Bot::Entity::Repositories {

using Utils::Entity::ApiRequest::InterfaceApiRequestRepository;
using Message::InterfaceMessageRepository;
using User::InterfaceUserRepository;
using Chat::InterfaceChatRepository;
using Access::InterfaceAccessRepository;
using Callback::InterfaceCallbackRepository;
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

    explicit Repositories(string_view conn_str);

    shared_ptr<InterfaceApiRequestRepository> api_request_repository;
    shared_ptr<InterfaceMessageRepository> message_repository;
    shared_ptr<InterfaceCallbackRepository> callback_repository;
    shared_ptr<InterfaceUserRepository> user_repository;
    shared_ptr<InterfaceChatRepository> chat_repository;
    shared_ptr<InterfaceAccessRepository> access_repository;

};

shared_ptr<Repositories> get_repositories(unique_ptr<Repositories> repositories = nullptr);

}
