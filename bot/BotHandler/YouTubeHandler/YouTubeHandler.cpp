#include <bot/BotHandler/YouTubeHandler/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <set>

namespace Bot::BotHandler::YouTubeHandler {

    using Entity::User::YOUTUBE;
    using Entity::User::SCREEN;
    using Entity::User::User;
    using Entity::Repositories::get_repositories;
    using Utils::TGBotApi::Bot::get_bot;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::ReplyButtons;
    using std::make_shared;
    using std::make_unique;
    using std::set;

    static ptrMessage YouTubeHandler::to_youtube(shared_ptr<BotHandlerContext> context) {
        User user;
        user.id = context->user->id;
        user.screen = YOUTUBE;
        get_repositories()->user_repository->update(user, {SCREEN});
        return get_bot()->send_message( {
            .chat_id = context->chat->telegram_id,
            .text = YOUTUBE_WORD,
            .reply_message_id = context->message->telegram_id,
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
        };
        return (context->access.full || context->access.youtube)
        && context->user->screen == YOUTUBE
        && buttons.contains(context->message->text)
        ;
    }

    ptrMessage YouTubeHandler::handle(shared_ptr<BotHandlerContext> context) {
        return get_bot()->send_message( {
            .chat_id = context->chat->telegram_id,
            .text = context->message->text,
            .reply_message_id = context->message->telegram_id,
        });
    }


}