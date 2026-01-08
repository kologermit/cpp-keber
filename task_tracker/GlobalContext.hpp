#pragma once

#include <task_tracker/Entity/Task/InterfaceTaskRepository.hpp>
#include <task_tracker/Config/InterfaceConfig.hpp>
#include <utils/Logger/InterfaceLogger.hpp>
#include <utils/BotApi/InterfaceBotApi.hpp>

namespace TaskTracker {
    using std::shared_ptr;
    using TaskTracker::Entity::Task::InterfaceTaskRepository;
    using TaskTracker::Config::InterfaceConfig;
    using Utils::Logger::InterfaceLogger;
    using Utils::BotApi::InterfaceBotApi;

    struct DBContext {
        shared_ptr<InterfaceTaskRepository> task;
    };

    struct GlobalContext {
        shared_ptr<InterfaceLogger> logger;
        shared_ptr<InterfaceConfig> config;
        shared_ptr<DBContext> db;
        shared_ptr<InterfaceBotApi> bot_api;
    };
}