#pragma once

#include <utils/Entity/ApiRequest/InterfaceApiRequestRepository.hpp>
#include <bot/Entity/Message/InterfaceMessageRepository.hpp>
#include <bot/Entity/User/InterfaceUserRepository.hpp>
#include <bot/Entity/Chat/InterfaceChatRepository.hpp>

namespace Bot::Entity::Repositories {

using Utils::Entity::ApiRequest::InterfaceApiRequestRepository;
using Bot::Entity::Message::InterfaceMessageRepository;
using Bot::Entity::User::InterfaceUserRepository;
using Bot::Entity::Chat::InterfaceChatRepository;
using std::shared_ptr;

struct Repositories {

    shared_ptr<InterfaceApiRequestRepository> api_request_repository;
    shared_ptr<InterfaceMessageRepository> message_repository;
    shared_ptr<InterfaceUserRepository> user_repository;
    shared_ptr<InterfaceChatRepository> chat_repository;

};

}