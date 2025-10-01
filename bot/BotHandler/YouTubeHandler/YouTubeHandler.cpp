#include <bot/BotHandler/YouTubeHandler/YouTubeHandler.hpp>
#include <bot/BotHandler/YouTubeHandler/MediaHandler/YouTubeMediaHandler.hpp>
#include <bot/BotHandler/YouTubeHandler/PlaylistHandler/YouTubePlaylistHandler.hpp>
#include <bot/BotHandler/MenuHandler/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <set>

namespace Bot::BotHandler::YouTubeHandler {

    using Entity::User::YOUTUBE;
    using Entity::User::SCREEN;
    using Entity::User::User;
    using Entity::Repositories::get_repositories;
    using MediaHandler::YouTubeMediaHandler;
    using PlaylistHandler::YouTubePlaylistHandler;
    using Utils::TGBotApi::Bot::get_bot;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::ReplyButtons;
    using std::make_shared;
    using std::make_unique;
    using std::set;

    ptrMessage YouTubeHandler::to_youtube(shared_ptr<BotHandlerContext> context) {
        context->user->screen = YOUTUBE;
        get_repositories()->user_repository->update(*context->user);
        return get_bot()->send_message( {
            .chat_id = context->chat->id,
            .text = YOUTUBE_WORD,
            .reply_message_id = context->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{
                {make_shared<ReplyButton>(AUDIO_WORD), make_shared<ReplyButton>(VIDEO_WORD)},
                {make_shared<ReplyButton>(AUDIO_PLAYLIST_WORD), make_shared<ReplyButton>(VIDEO_PLAYLIST_WORD)},
                {make_shared<ReplyButton>(BACK_WORD)}
            })
        });
    }


    const string& YouTubeHandler::get_name() const noexcept {
        static const string name = "YouTubeHandler";
        return name;
    }

    bool YouTubeHandler::check(shared_ptr<BotHandlerContext> context) {
        static const set<string> buttons{
            AUDIO_WORD, VIDEO_WORD,
            AUDIO_PLAYLIST_WORD, VIDEO_PLAYLIST_WORD,
            BACK_WORD,
        };
        return (context->access.full || context->access.youtube)
        && context->user->screen == YOUTUBE
        && buttons.contains(context->message->text)
        ;
    }

    ptrMessage YouTubeHandler::handle(shared_ptr<BotHandlerContext> context) {
        if (context->message->text == BACK_WORD) {
            return MenuHandler::MenuHandler::to_menu(context);
        }

        if (context->message->text == VIDEO_WORD || context->message->text == AUDIO_WORD) {
            return YouTubeMediaHandler::to_youtube_media(context, context->message->text == VIDEO_WORD);
        }

        if (context->message->text == AUDIO_PLAYLIST_WORD || context->message->text == VIDEO_PLAYLIST_WORD) {
            return YouTubePlaylistHandler::to_youtube_playlist(context, context->message->text == VIDEO_PLAYLIST_WORD);
        }

        return get_bot()->send_message( {
            .chat_id = context->chat->id,
            .text = IN_DEVELOP_PHRASE,
            .reply_message_id = context->message->id,
        });
    }


}
