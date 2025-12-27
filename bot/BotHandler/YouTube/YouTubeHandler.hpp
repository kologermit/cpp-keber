#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>
#include <utils/Config/InterfaceConfig.hpp>
#include <utils/RabbitMQ/Queue/Queue.hpp>

namespace Bot::BotHandler::YouTube {

    using std::shared_ptr;
    using std::make_shared;
    using Utils::RabbitMQ::Queue;
    using Utils::Config::InterfaceConfig;

    enum EnumDownloadType {
        AUDIO,
        VIDEO_720P, VIDEO_BEST,
        PLAYLIST_AUDIO,
        PLAYLIST_VIDEO_720P, PLAYLIST_VIDEO_BEST,
    };

    struct YouTubeHandler final : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(shared_ptr<BotHandlerContext> context) override;
        ptrMessage handle(shared_ptr<BotHandlerContext> context) override;

        static ptrMessage to_youtube(shared_ptr<BotHandlerContext> context);
    };

    shared_ptr<Queue> get_downloader_queue(shared_ptr<InterfaceConfig> config) {
        static shared_ptr<Queue> downloader = nullptr;

        if (downloader == nullptr) {
            downloader = make_shared<Queue>(
                config->get_downloader_queue_name(),
                config->get_rabbit_mq_vhost(),
                config->get_rabbit_mq_user(),
                config->get_rabbit_mq_password(),
                config->get_rabbit_mq_host(),
                config->get_rabbit_mq_port()
            );
        }

        if (!downloader->exists()) {
            downloader->declare();
        }

        return downloader;
    }
}