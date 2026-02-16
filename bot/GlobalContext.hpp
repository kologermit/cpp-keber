#pragma once

#include <bot/Entity/Access/InterfaceAccessRepository.hpp>
#include <bot/Entity/Callback/InterfaceCallbackRepository.hpp>
#include <bot/Entity/Chat/InterfaceChatRepository.hpp>
#include <bot/Entity/Message/InterfaceMessageRepository.hpp>
#include <bot/Entity/User/InterfaceUserRepository.hpp>
#include <bot/Entity/YouTubeAudioSetting/InterfaceYouTubeAudioSettingRepository.hpp>
#include <bot/Config/InterfaceConfig.hpp>
#include <utils/Logger/InterfaceLogger.hpp>
#include <utils/YouTubeApi/InterfaceYouTubeApi.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <utils/TaskTrackerApi/InterfaceTaskTrackerApi.hpp>

namespace Bot {
    using std::shared_ptr;
    using std::string;
    using std::map;
    using jed_utils::datetime;
    using Utils::Logger::InterfaceLogger;
    using Utils::YouTubeApi::InterfaceYouTubeApi;
    using Utils::TaskTrackerApi::InterfaceTaskTrackerApi;
    using Utils::TGBotApi::Bot::InterfaceBot;
    using Bot::Config::InterfaceConfig;
    using Bot::Entity::Access::InterfaceAccessRepository;
    using Bot::Entity::Callback::InterfaceCallbackRepository;
    using Bot::Entity::Chat::InterfaceChatRepository;
    using Bot::Entity::Message::InterfaceMessageRepository;
    using Bot::Entity::User::InterfaceUserRepository;
    using Bot::Entity::YouTubeAudioSetting::InterfaceYouTubeAudioSettingRepository;

    struct MinimalTask {
        string title = "";
        string description = "";
        datetime start_at = {};
    };

    struct ApiContext {
        shared_ptr<InterfaceTaskTrackerApi> task_tracker;
        shared_ptr<InterfaceYouTubeApi> youtube;
    };

    struct DBContext {
        shared_ptr<InterfaceAccessRepository> access;
        shared_ptr<InterfaceCallbackRepository> callback;
        shared_ptr<InterfaceChatRepository> chat;
        shared_ptr<InterfaceMessageRepository> message;
        shared_ptr<InterfaceUserRepository> user;
        shared_ptr<InterfaceYouTubeAudioSettingRepository> youtube_audio_setting;
    };

    struct GlobalContext {
        shared_ptr<map<long long, MinimalTask>> task_tracker_cache;
        shared_ptr<InterfaceLogger> logger;
        shared_ptr<InterfaceConfig> config;
        shared_ptr<InterfaceBot> bot;
        shared_ptr<DBContext> db;
        shared_ptr<ApiContext> api;
    };
}