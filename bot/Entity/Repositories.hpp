#pragma once

#include <utils/Entity/ApiRequest/InterfaceApiRequestRepository.hpp>
#include <bot/Entity/Message/InterfaceMessageRepository.hpp>
#include <bot/Entity/User/InterfaceUserRepository.hpp>
#include <bot/Entity/Chat/InterfaceChatRepository.hpp>
#include <bot/Entity/Access/InterfaceAccessRepository.hpp>
#include <bot/Entity/Callback/InterfaceCallbackRepository.hpp>
#include <bot/Entity/YouTubeAudioSetting/InterfaceYouTubeAudioSettingRepository.hpp>
#include <pqxx/pqxx>

namespace Bot::Entity::Repositories {

    using Utils::Entity::ApiRequest::InterfaceApiRequestRepository;
    using Bot::Entity::Message::InterfaceMessageRepository;
    using Bot::Entity::User::InterfaceUserRepository;
    using Bot::Entity::Chat::InterfaceChatRepository;
    using Bot::Entity::Access::InterfaceAccessRepository;
    using Bot::Entity::Callback::InterfaceCallbackRepository;
    using Bot::Entity::YouTubeAudioSetting::InterfaceYouTubeAudioSettingRepository;
    using pqxx::connection;
    using std::shared_ptr;
    using std::unique_ptr;
    using std::string_view;

    class Repositories {

        connection _db;

        public:

        explicit Repositories(string_view conn_str);

        shared_ptr<InterfaceApiRequestRepository> api_request;
        shared_ptr<InterfaceMessageRepository> message;
        shared_ptr<InterfaceCallbackRepository> callback;
        shared_ptr<InterfaceUserRepository> user;
        shared_ptr<InterfaceChatRepository> chat;
        shared_ptr<InterfaceAccessRepository> access;
        shared_ptr<InterfaceYouTubeAudioSettingRepository> youtube_audio_setting;

    };

    shared_ptr<Repositories> get_repositories(unique_ptr<Repositories> repositories = nullptr);

}
