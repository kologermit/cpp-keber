#include <bot/BotHandler/YouTubeHandler/MediaHandler/YouTubeMediaHandler.hpp>
#include <bot/BotHandler/YouTubeHandler/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <utils/YouTubeApi/InterfaceYouTubeApi.hpp>
#include <fmt/core.h>
#include <sstream>

namespace Bot::BotHandler::YouTubeHandler::MediaHandler {

    using Bot::Entity::Repositories::get_repositories;
    using Bot::Entity::User::User;
    using Bot::Entity::User::EnumUserScreen;
    using Bot::Entity::YouTubeAudioSetting::YouTubeAudioSetting;
    using Utils::TGBotApi::Bot::get_bot;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButtons;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::InlineKeyboard;
    using Utils::TGBotApi::Types::InlineButtons;
    using Utils::TGBotApi::Types::InlineButton;
    using Utils::YouTubeApi::get_youtube_api;
    using fmt::format;
    using nlohmann::json;
    using std::vector;
    using std::unique_ptr;
    using std::make_unique;
    using std::make_shared;
    using std::istringstream;
    using std::getline;

    ptrMessage YouTubeMediaHandler::to_youtube_media(shared_ptr<BotHandlerContext> context, bool is_video) {
        static const string VIDEO_TEXT = format(SEND_MEDIA_URL_PHRASE, VIDEO_WORD);
        static const string AUDIO_TEXT = format(SEND_MEDIA_URL_PHRASE, AUDIO_WORD);
        static const ReplyButtons VIDEO_KEYBOARD{{make_shared<ReplyButton>(BACK_WORD)}};
        static const ReplyButtons AUDIO_KEYBOARD{{make_shared<ReplyButton>(SETTINGS_TABLE_WORD), make_shared<ReplyButton>(BACK_WORD)}};

        context->user->screen = (is_video ? EnumUserScreen::YOUTUBE_VIDEO : EnumUserScreen::YOUTUBE_AUDIO);
        get_repositories()->user->update(*context->user);
        return get_bot()->send_message( {
            .chat_id = context->chat->id,
            .text = (is_video ? VIDEO_TEXT : AUDIO_TEXT),
            .reply_message_id = context->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(is_video ? VIDEO_KEYBOARD : AUDIO_KEYBOARD)
        });
    }

    const string& YouTubeMediaHandler::get_name() const noexcept {
        static const string name = "YouTubeMediaHandler";
        return name;
    }

    bool YouTubeMediaHandler::check(shared_ptr<BotHandlerContext> context) {
        return (context->access.full || context->access.youtube)
        && (context->user->screen == EnumUserScreen::YOUTUBE_VIDEO || context->user->screen == EnumUserScreen::YOUTUBE_AUDIO)
        && (context->message->text.starts_with("https") || context->message->text == BACK_WORD);
    }

    ptrMessage YouTubeMediaHandler::handle(shared_ptr<BotHandlerContext> context) {
        if (context->message->text == BACK_WORD) {
            return YouTubeHandler::to_youtube(context);
        }
        if (context->user->screen == EnumUserScreen::YOUTUBE_AUDIO && context->message->text == SETTINGS_TABLE_WORD) {
            return send_audio_settings(context);
        }
        if (context->user->screen == EnumUserScreen::YOUTUBE_AUDIO && context->message->file_name.ends_with(".xlsx")) {
            return handle_audio_settings(context);
        }
        return handle_urls(context);
    }

    ptrMessage YouTubeMediaHandler::send_audio_settings(shared_ptr<BotHandlerContext> context) {
        auto settings = get_repositories()->youtube_audio_setting->get_by_user_id(context->user->id);

        // TODO: почитать докумантацию по libxlsxwriter и написать отправку таблицы с настройками

        return nullptr;
    }

    ptrMessage YouTubeMediaHandler::handle_audio_settings(shared_ptr<BotHandlerContext> context) {
        // TODO: почитать докумантацию по libxlsxwriter и написать обработку таблицы с настройками
        vector<unique_ptr<YouTubeAudioSetting> > settings;
        settings.emplace_back(make_unique<YouTubeAudioSetting>(context->user->id, "123", "123", "123"));
        get_repositories()->youtube_audio_setting->update_by_user_id(context->user->id, std::move(settings));
        return nullptr;
    }

    ptrMessage YouTubeMediaHandler::handle_urls(shared_ptr<BotHandlerContext> context) {
        const char* MEDIA_TYPE_WORD = (
            context->user->screen == EnumUserScreen::YOUTUBE_VIDEO
            ? VIDEO_WORD
            : AUDIO_WORD
        );
        istringstream stream(context->message->text);
        string buffer, answer;
        int count = 0;
        while (getline(stream, buffer)) {
            if (buffer.empty()) {
                continue;
            }
            auto result = get_youtube_api()->get_video(buffer);
            if (result == nullptr) {
                return get_bot()->send_message( {
                    .chat_id = context->chat->id,
                    .text = format(MEDIA_NOT_FOUND_PHRASE, MEDIA_TYPE_WORD, buffer),
                    .reply_message_id = context->message->id,
                });
            }
            const auto& [video, channel] = *result;
            answer += "\n\n" +  format(MEDIA_TEMPLATE, video.title, channel.title, video.video_url);
            count++;
        }

        return get_bot()->send_message( {
            .chat_id = context->chat->id,
            .text = format(DOWNLOAD_MEDIA_PHRASE,
                MEDIA_TYPE_WORD,
                count,
                answer
            ),
            .reply_message_id = context->message->id,
            .inline_keyboard = make_unique<InlineKeyboard>(
                context->user->screen == EnumUserScreen::YOUTUBE_VIDEO
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