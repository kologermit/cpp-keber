#pragma once

#include <bot/Entity/Access/InterfaceAccessRepository.hpp>
#include <bot/Entity/Callback/InterfaceCallbackRepository.hpp>
#include <bot/Entity/Chat/InterfaceChatRepository.hpp>
#include <bot/Entity/Message/InterfaceMessageRepository.hpp>
#include <bot/Entity/User/InterfaceUserRepository.hpp>
#include <bot/Entity/YouTubeAudioSetting/InterfaceYouTubeAudioSettingRepository.hpp>
#include <bot/Config/InterfaceConfig.hpp>
#include <bot/MessageThread/InterfaceMessageThread.hpp>
#include <utils/HTTPServer/Server/InterfaceServer.hpp>
#include <utils/Logger/InterfaceLogger.hpp>
#include <utils/YouTubeApi/InterfaceYouTubeApi.hpp>
#include <utils/TGBotApi/Bot/InterfaceBot.hpp>
#include <utils/TaskTrackerApi/InterfaceTaskTrackerApi.hpp>
#include <thread>
#include <queue>
#include <mutex>

namespace Bot {
    using std::shared_ptr;
    using std::unique_ptr;
    using std::string;
    using std::map;
    using std::pair;
    using std::queue;
    using std::mutex;
    using std::jthread;
    using std::condition_variable;
    using std::optional;
    using jed_utils::datetime;
    using Utils::Logger::InterfaceLogger;
    using Utils::YouTubeApi::InterfaceYouTubeApi;
    using Utils::TaskTrackerApi::InterfaceTaskTrackerApi;
    using Utils::TGBotApi::Bot::InterfaceBot;
    using TGMessage = Utils::TGBotApi::Types::Message;
    using TGCallbackQuery = Utils::TGBotApi::Types::CallbackQuery;
    using Bot::Config::InterfaceConfig;
    using Bot::Entity::Access::InterfaceAccessRepository;
    using Bot::Entity::Callback::InterfaceCallbackRepository;
    using Bot::Entity::Chat::InterfaceChatRepository;
    using Bot::Entity::Message::InterfaceMessageRepository;
    using Bot::Entity::User::InterfaceUserRepository;
    using Bot::Entity::YouTubeAudioSetting::InterfaceYouTubeAudioSettingRepository;
    using Bot::MessageThread::InterfaceMessageThread;

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

    struct WorkerTask {
        const int id;
        optional<TGMessage> tg_message;
        optional<TGCallbackQuery> tg_callback_query;
    };

    struct GlobalContext {
        shared_ptr<map<long long, MinimalTask>> task_tracker_cache;
        shared_ptr<queue<unique_ptr<WorkerTask> > > message_queue;
        shared_ptr<mutex> message_queue_mutex;
        shared_ptr<condition_variable> message_queue_cv;
        shared_ptr<InterfaceLogger> logger;
        shared_ptr<InterfaceConfig> config;
        shared_ptr<InterfaceBot> bot;
        shared_ptr<DBContext> db;
        shared_ptr<ApiContext> api;
    };
}

extern std::shared_ptr<Bot::GlobalContext> global_context;