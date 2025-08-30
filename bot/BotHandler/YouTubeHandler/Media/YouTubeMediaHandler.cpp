#include <bot/BotHandler/YouTubeHandler/Media/YouTubeMediaHandler.hpp>
#include <bot/BotHandler/YouTubeHandler/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <utils/YouTubeApi/InterfaceYouTubeApi.hpp>
#include <fmt/core.h>
#include <sstream>

namespace Bot::BotHandler::YouTubeHandler::Media {

    using Bot::Entity::Repositories::get_repositories;
    using Bot::Entity::User::User;
    using Bot::Entity::User::EnumUserScreen::YOUTUBE_VIDEO;
    using Bot::Entity::User::EnumUserScreen::YOUTUBE_AUDIO;
    using Bot::Entity::User::UserColumns::SCREEN;
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

    ptrMessage YouTubeMediaHandler::to_youtube_media(shared_ptr<BotHandlerContext> context, bool is_video) {
        User user;
        user.id = context->user->id;
        user.screen = (is_video ? YOUTUBE_VIDEO : YOUTUBE_AUDIO);
        get_repositories()->user_repository->update(user, {SCREEN});
        return get_bot()->send_message( {
            .chat_id = context->chat->telegram_id,
            .text = format(SEND_MEDIA_URL_PHRASE, (is_video ? VIDEO_WORD : AUDIO_WORD)),
            .reply_message_id = context->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{{make_shared<ReplyButton>(BACK_WORD)}})
        });
    }

    const string& YouTubeMediaHandler::get_name() const noexcept {
        static const string name = "YouTubeMediaHandler";
        return name;
    }

    bool YouTubeMediaHandler::check(shared_ptr<BotHandlerContext> context) {
        return (context->access.full || context->access.youtube)
        && (context->user->screen == YOUTUBE_VIDEO || context->user->screen == YOUTUBE_AUDIO)
        && (context->message->text.starts_with("https") || context->message->text == BACK_WORD);
    }

    ptrMessage YouTubeMediaHandler::handle(shared_ptr<BotHandlerContext> context) {
        if (context->message->text == BACK_WORD) {
            return YouTubeHandler::to_youtube(context);
        }

        const char* MEDIA_TYPE_WORD = (
            context->user->screen == YOUTUBE_VIDEO
            ? VIDEO_WORD
            : AUDIO_WORD
        );
        istringstream stream(context->message->text);
        string buffer, answer;
        int count = 0;
        for (; getline(stream, buffer); count++) {
            if (buffer.empty()) {
                continue;
            }
            auto result = get_youtube_api()->get_video(buffer);
            if (result == nullptr) {
                return get_bot()->send_message( {
                    .chat_id = context->chat->telegram_id,
                    .text = format(MEDIA_NOT_FOUND_PHRASE, MEDIA_TYPE_WORD, buffer),
                    .reply_message_id = context->message->telegram_id,
                });
            }
            const auto& [video, channel] = *result;
            answer += "\n\n" +  format(MEDIA_TEMPLATE, video.title, channel.title, video.video_url);
        }

        return get_bot()->send_message( {
            .chat_id = context->chat->telegram_id,
            .text = format(DOWNLOAD_MEDIA_PHRASE,
                MEDIA_TYPE_WORD,
                count,
                answer
            ),
            .reply_message_id = context->message->telegram_id,
            .inline_keyboard = make_unique<InlineKeyboard>(context->user->screen == YOUTUBE_VIDEO
                ? InlineButtons{
                    {make_shared<InlineButton>(DELETE_WORD, "", "delete")},
                    {
                        make_shared<InlineButton>(ADD_720P_QUALITY_PHRASE, "", json{
                            YOUTUBE_MEDIA_CALLBACK_HANDLER_NAME,
                            VIDEO_720P,
                        }.dump()),
                        make_shared<InlineButton>(ADD_BEST_QUALITY_PHRASE, "", json{
                            YOUTUBE_MEDIA_CALLBACK_HANDLER_NAME,
                            VIDEO_BEST,
                        }.dump())
                    }
                }
                : InlineButtons{
                    {
                        make_shared<InlineButton>(DELETE_WORD, "", "delete"),
                        make_shared<InlineButton>(ADD_WORD, "", json{
                            YOUTUBE_MEDIA_CALLBACK_HANDLER_NAME,
                            AUDIO,
                        }.dump())
                    }
                }
            )
        });
    }
}