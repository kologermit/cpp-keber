#include <bot/BotHandler/YouTube/YouTubeHandler.hpp>
#include <bot/BotHandler/YouTube/Media/MediaHandler.hpp>
#include <bot/BotHandler/YouTube/Playlist/PlaylistHandler.hpp>
#include <bot/BotHandler/Menu/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <set>

namespace Bot::BotHandler::YouTube {

    using Entity::User::User;
    using Entity::User::UserScreen;
    using Bot::BotHandler::Menu::MenuHandler;
    using Bot::BotHandler::YouTube::Media::MediaHandler;
    using Bot::BotHandler::YouTube::Playlist::PlaylistHandler;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::ReplyButtons;
    using std::make_shared;
    using std::make_unique;
    using std::runtime_error;
    using std::set;

    ptrMessage YouTubeHandler::to_youtube(shared_ptr<BotHandlerContext> ctx) {
        ctx->user->screen = UserScreen::YOUTUBE;
        ctx->db->user->update(*ctx->user);
        return ctx->bot->send_message( {
            .chat_id = ctx->chat->id,
            .text = YOUTUBE_WORD,
            .reply_message_id = ctx->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{
                {make_shared<ReplyButton>(AUDIO_WORD), make_shared<ReplyButton>(VIDEO_WORD)},
                {make_shared<ReplyButton>(AUDIO_PLAYLIST_WORD), make_shared<ReplyButton>(VIDEO_PLAYLIST_WORD)},
                {make_shared<ReplyButton>(BACK_WORD)}
            })
        });
    }

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
            if (!downloader->declare()) {
                throw runtime_error(
                    fmt::format(
                        fmt::runtime("Failed to declare queue {}"),
                        config->get_downloader_queue_name()
                    )
                );
            }
        }

        return downloader;
    }


    const string& YouTubeHandler::get_name() const noexcept {
        static const string name = "YouTubeHandler";
        return name;
    }

    bool YouTubeHandler::check(shared_ptr<BotHandlerContext> ctx) {
        static const set<string> buttons{
            AUDIO_WORD, VIDEO_WORD,
            AUDIO_PLAYLIST_WORD, VIDEO_PLAYLIST_WORD,
            BACK_WORD,
        };
        return (ctx->access.full || ctx->access.youtube)
        && ctx->user->screen == UserScreen::YOUTUBE
        && buttons.contains(ctx->message->text)
        ;
    }

    ptrMessage YouTubeHandler::handle(shared_ptr<BotHandlerContext> ctx) {
        if (ctx->message->text == BACK_WORD) {
            return MenuHandler::to_menu(ctx);
        }

        if (ctx->message->text == VIDEO_WORD || ctx->message->text == AUDIO_WORD) {
            return MediaHandler::to_youtube_media(ctx, ctx->message->text == VIDEO_WORD);
        }

        if (ctx->message->text == AUDIO_PLAYLIST_WORD || ctx->message->text == VIDEO_PLAYLIST_WORD) {
            return PlaylistHandler::to_youtube_playlist(ctx, ctx->message->text == VIDEO_PLAYLIST_WORD);
        }

        return ctx->bot->send_message( {
            .chat_id = ctx->chat->id,
            .text = IN_DEVELOP_PHRASE,
            .reply_message_id = ctx->message->id,
        });
    }


}
