#include <bot/BotHandler/YouTubeHandler/PlaylistHandler/YouTubePlaylistHandler.hpp>
#include <bot/BotHandler/YouTubeHandler/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <utils/YouTubeApi/YouTubeApi.hpp>
#include <fmt/format.h>

namespace Bot::BotHandler::YouTubeHandler::PlaylistHandler {

    using Bot::Entity::Repositories::get_repositories;
    using Bot::Entity::User::User;
    using Bot::Entity::User::EnumUserScreen::YOUTUBE_PLAYLIST_VIDEO;
    using Bot::Entity::User::EnumUserScreen::YOUTUBE_PLAYLIST_AUDIO;
    using Bot::Entity::User::UserColumns::SCREEN;
    using Bot::Entity::Repositories::get_repositories;
    using Utils::TGBotApi::Bot::get_bot;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButtons;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::InlineKeyboard;
    using Utils::TGBotApi::Types::InlineButtons;
    using Utils::TGBotApi::Types::InlineButton;
    using Utils::YouTubeApi::get_youtube_api;
    using Utils::YouTubeApi::Video;
    using Utils::YouTubeApi::Channel;
    using fmt::format;
    using nlohmann::json;
    using std::unique_ptr;
    using std::make_unique;
    using std::make_shared;
    using std::istringstream;
    using std::getline;
    using std::pair;
    using std::move;

    ptrMessage YouTubePlaylistHandler::to_youtube_playlist(shared_ptr<BotHandlerContext> context, bool is_video) {
        User user;
        user.id = context->user->id;
        user.screen = (is_video ? YOUTUBE_PLAYLIST_VIDEO : YOUTUBE_PLAYLIST_AUDIO);
        get_repositories()->user_repository->update(user, {SCREEN});

        return get_bot()->send_message( {
            .chat_id = context->chat->telegram_id,
            .text = format(SEND_MEDIA_URL_PHRASE, (is_video ? VIDEO_PLAYLIST_WORD : AUDIO_PLAYLIST_WORD)),
            .reply_message_id = context->message->telegram_id,
            .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{{make_shared<ReplyButton>(BACK_WORD)}})
        });
    }

    const string& YouTubePlaylistHandler::get_name() const noexcept {
        static const string name = "YouTubePlaylistHandler";
        return name;
    }

    bool YouTubePlaylistHandler::check(shared_ptr<BotHandlerContext> context) {
        return (context->access.full || context->access.youtube)
        && (context->user->screen == YOUTUBE_PLAYLIST_VIDEO || context->user->screen == YOUTUBE_PLAYLIST_AUDIO)
        && (context->message->text.starts_with("https://") || context->message->text == BACK_WORD);
    }

    ptrMessage YouTubePlaylistHandler::handle(shared_ptr<BotHandlerContext> context) {
        if (context->message->text == BACK_WORD) {
            return YouTubeHandler::to_youtube(context);
        }

        return get_bot()->send_message( {
            .chat_id = context->chat->telegram_id,
            .text = IN_DEVELOP_PHRASE,
            .reply_message_id = context->message->telegram_id,
        });
    }

}