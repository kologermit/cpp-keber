#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>
#include <utils/Config/InterfaceConfig.hpp>
#include <utils/RabbitMQ/Queue/Queue.hpp>

namespace Bot::BotHandler::YouTubeHandler {

    using std::shared_ptr;
    using std::make_shared;
    using Utils::Config::get_config;
    using Utils::RabbitMQ::Queue;

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

    shared_ptr<Queue> get_downloader_queue() {
        static shared_ptr<Queue> downloader = nullptr;

        if (downloader == nullptr) {
            downloader = make_shared<Queue>(
                get_config()->get_downloader_queue_name(),
                get_config()->get_rabbit_mq_vhost(),
                get_config()->get_rabbit_mq_user(),
                get_config()->get_rabbit_mq_password(),
                get_config()->get_rabbit_mq_host(),
                get_config()->get_rabbit_mq_port()
            );
        }

        if (!downloader->exists()) {
            downloader->declare();
        }

        return downloader;
    }
}