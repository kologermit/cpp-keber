#include <bot/BotHandler/YouTube/YouTubeHandler.hpp>
#include <bot/BotHandler/YouTube/Media/MediaHandler.hpp>
#include <bot/BotHandler/YouTube/Playlist/PlaylistHandler.hpp>
#include <bot/BotHandler/Menu/MenuHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <set>

namespace Bot::BotHandler::YouTube {

    using Entity::User::YOUTUBE;
    using Entity::User::SCREEN;
    using Entity::User::User;
    using Bot::BotHandler::Menu::MenuHandler;
    using Bot::BotHandler::YouTube::Media::MediaHandler;
    using Bot::BotHandler::YouTube::Playlist::PlaylistHandler;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::ReplyButtons;
    using std::make_shared;
    using std::make_unique;
    using std::set;

    ptrMessage YouTubeHandler::to_youtube(shared_ptr<BotHandlerContext> context) {
        context->user->screen = YOUTUBE;
        context->global_context->user_repository->update(*context->user);
        return context->bot->send_message( {
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
            return MenuHandler::to_menu(context);
        }

        if (context->message->text == VIDEO_WORD || context->message->text == AUDIO_WORD) {
            return MediaHandler::to_youtube_media(context, context->message->text == VIDEO_WORD);
        }

        if (context->message->text == AUDIO_PLAYLIST_WORD || context->message->text == VIDEO_PLAYLIST_WORD) {
            return PlaylistHandler::to_youtube_playlist(context, context->message->text == VIDEO_PLAYLIST_WORD);
        }

        return context->bot->send_message( {
            .chat_id = context->chat->id,
            .text = IN_DEVELOP_PHRASE,
            .reply_message_id = context->message->id,
        });
    }


}
