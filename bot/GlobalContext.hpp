#pragma once

#include <bot/Entity/Access/InterfaceAccessRepository.hpp>
#include <bot/Entity/Callback/InterfaceCallbackRepository.hpp>
#include <bot/Entity/Chat/InterfaceChatRepository.hpp>
#include <bot/Entity/Message/InterfaceMessageRepository.hpp>
#include <bot/Entity/User/InterfaceUserRepository.hpp>
#include <bot/Entity/YouTubeAudioSetting/InterfaceYouTubeAudioSettingRepository.hpp>
#include <utils/Logger/InterfaceLogger.hpp>
#include <utils/Config/InterfaceConfig.hpp>
#include <utils/YouTubeApi/InterfaceYouTubeApi.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>

namespace Bot {
    using std::shared_ptr;
    using std::string;
    using Utils::Logger::InterfaceLogger;
    using Utils::Config::InterfaceConfig;
    using Utils::YouTubeApi::InterfaceYouTubeApi;
    using Utils::TGBotApi::Bot::InterfaceBot;
    using Bot::Entity::Access::InterfaceAccessRepository;
    using Bot::Entity::Callback::InterfaceCallbackRepository;
    using Bot::Entity::Chat::InterfaceChatRepository;
    using Bot::Entity::Message::InterfaceMessageRepository;
    using Bot::Entity::User::InterfaceUserRepository;
    using Bot::Entity::YouTubeAudioSetting::InterfaceYouTubeAudioSettingRepository;

    struct GlobalContext {
        shared_ptr<InterfaceLogger> logger;
        shared_ptr<InterfaceConfig> config;
        shared_ptr<InterfaceBot> bot;
        shared_ptr<InterfaceAccessRepository> access_repository;
        shared_ptr<InterfaceCallbackRepository> callback_repository;
        shared_ptr<InterfaceChatRepository> chat_repository;
        shared_ptr<InterfaceMessageRepository> message_repository;
        shared_ptr<InterfaceUserRepository> user_repository;
        shared_ptr<InterfaceYouTubeAudioSettingRepository> youtube_audio_setting_repository;
        shared_ptr<InterfaceYouTubeApi> youtube_api;
        const string auth_key;
    };
}