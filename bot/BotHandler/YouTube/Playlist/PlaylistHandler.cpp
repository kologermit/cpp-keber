#include <bot/BotHandler/YouTube/Playlist/PlaylistHandler.hpp>
#include <bot/BotHandler/YouTube/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <utils/YouTubeApi/YouTubeApi.hpp>
#include <fmt/format.h>

namespace Bot::BotHandler::YouTube::Playlist {
    using Bot::Entity::User::User;
    using Bot::Entity::User::EnumUserScreen;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButtons;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::InlineKeyboard;
    using Utils::TGBotApi::Types::InlineButtons;
    using Utils::TGBotApi::Types::InlineButton;
    using fmt::format;
    using nlohmann::json;
    using std::make_unique;
    using std::make_shared;
    using std::istringstream;
    using std::getline;

    ptrMessage PlaylistHandler::to_youtube_playlist(shared_ptr<BotHandlerContext> context, bool is_video) {
        context->user->screen = (is_video ? EnumUserScreen::YOUTUBE_PLAYLIST_VIDEO : EnumUserScreen::YOUTUBE_PLAYLIST_AUDIO);
        context->global_context->user_repository->update(*context->user);

        return context->bot->send_message( {
            .chat_id = context->chat->id,
            .text = format(SEND_MEDIA_URL_PHRASE, (is_video ? VIDEO_PLAYLIST_WORD : AUDIO_PLAYLIST_WORD)),
            .reply_message_id = context->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(ReplyButtons{{make_shared<ReplyButton>(BACK_WORD)}})
        });
    }

    const string& PlaylistHandler::get_name() const noexcept {
        static const string name = "YouTubePlaylistHandler";
        return name;
    }

    bool PlaylistHandler::check(shared_ptr<BotHandlerContext> context) {
        return (context->access.full || context->access.youtube)
        && (context->user->screen == EnumUserScreen::YOUTUBE_PLAYLIST_VIDEO || context->user->screen == EnumUserScreen::YOUTUBE_PLAYLIST_AUDIO)
        && (context->message->text.starts_with("https://") || context->message->text == BACK_WORD);
    }

    ptrMessage PlaylistHandler::handle(shared_ptr<BotHandlerContext> context) {
        if (context->message->text == BACK_WORD) {
            return YouTubeHandler::to_youtube(context);
        }

        const char* PLAYLIST_TYPE_WORD = (
            context->user->screen == EnumUserScreen::YOUTUBE_PLAYLIST_VIDEO
            ? VIDEO_PLAYLIST_WORD
            : AUDIO_PLAYLIST_WORD
        );

        istringstream stream(context->message->text);
        string buffer, answer;
        int count = 0;
        while (getline(stream, buffer)) {
            if (buffer.empty()) {
                continue;
            }
            auto playlist = context->global_context->youtube_api->get_playlist(buffer);
            if (playlist == nullptr) {
                return context->bot->send_message( {
                    .chat_id = context->chat->id,
                    .text = format(MEDIA_NOT_FOUND_PHRASE, PLAYLIST_TYPE_WORD, buffer),
                    .reply_message_id = context->message->id,
                });
            }

            answer += "\n\n" + format(MEDIA_TEMPLATE,
                playlist->title,
                playlist->video_urls.size(),
                playlist->playlist_url
            );

            count++;
        }

        return context->bot->send_message( {
            .chat_id = context->chat->id,
            .text = format(DOWNLOAD_MEDIA_PHRASE,
                PLAYLIST_TYPE_WORD,
                count,
                answer
            ),
            .reply_message_id = context->message->id,
            .inline_keyboard = make_unique<InlineKeyboard>(context->user->screen == EnumUserScreen::YOUTUBE_PLAYLIST_VIDEO
                ? InlineButtons{
                    {make_shared<InlineButton>(DELETE_WORD, "", "delete")},
                    {
                        make_shared<InlineButton>(ADD_720P_QUALITY_PHRASE, "", json{
                            YOUTUBE_PLAYLIST_CALLBACK_HANDLER_NAME,
                            VIDEO_720P,
                        }.dump()),
                        make_shared<InlineButton>(ADD_BEST_QUALITY_PHRASE, "", json{
                            YOUTUBE_PLAYLIST_CALLBACK_HANDLER_NAME,
                            VIDEO_BEST,
                        }.dump())
                    }
                }
                : InlineButtons{
                    {
                        make_shared<InlineButton>(DELETE_WORD, "", "delete"),
                        make_shared<InlineButton>(ADD_WORD, "", json{
                            YOUTUBE_PLAYLIST_CALLBACK_HANDLER_NAME,
                            AUDIO,
                        }.dump())
                    }
                }
            )
        });
    }
}