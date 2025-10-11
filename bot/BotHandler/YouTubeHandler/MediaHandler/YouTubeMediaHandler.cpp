#include <bot/BotHandler/YouTubeHandler/MediaHandler/YouTubeMediaHandler.hpp>
#include <bot/BotHandler/YouTubeHandler/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Entity/Repositories.hpp>
#include <utils/Config/InterfaceConfig.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <utils/YouTubeApi/InterfaceYouTubeApi.hpp>
#include <utils/XLSX/XLSX.hpp>
#include <fmt/core.h>
#include <sstream>

namespace Bot::BotHandler::YouTubeHandler::MediaHandler {

    using Bot::Entity::Repositories::get_repositories;
    using Bot::Entity::User::User;
    using Bot::Entity::User::EnumUserScreen;
    using Bot::Entity::YouTubeAudioSetting::YouTubeAudioSetting;
    using Utils::Config::get_config;
    using Utils::TGBotApi::Bot::get_bot;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButtons;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::InlineKeyboard;
    using Utils::TGBotApi::Types::InlineButtons;
    using Utils::TGBotApi::Types::InlineButton;
    using Utils::TGBotApi::File::DOCUMENT;
    using Utils::YouTubeApi::get_youtube_api;
    using Utils::XLSX::read_xlsx;
    using Utils::XLSX::write_xlsx;
    using Utils::XLSX::CellCoords;
    using fmt::format;
    using nlohmann::json;
    using std::to_string;
    using std::map;
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
        return (
            context->access.full 
            || context->access.youtube
        ) && (
            context->user->screen == EnumUserScreen::YOUTUBE_VIDEO 
            || context->user->screen == EnumUserScreen::YOUTUBE_AUDIO
        ) && (
            context->message->text.starts_with("https")
            || context->message->text == BACK_WORD
            || context->message->text == SETTINGS_TABLE_WORD
            || context->message->file_name.ends_with(".xlsx")
        );
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

        map<CellCoords, string> table;

        table[{0, 0}] = "# (необязательно)";
        table[{0, 1}] = "Название файл (необязательно)";
        table[{0, 2}] = "Ссылка Ютуб";
        table[{0, 3}] = "Ссылка для скачивания";

        for (size_t i = 0; i < settings.size(); i++) {
            table[{static_cast<unsigned short>(i+1), 0}] = to_string(i+1);
            table[{static_cast<unsigned short>(i+1), 1}] = settings[i]->file_name;
            table[{static_cast<unsigned short>(i+1), 2}] = settings[i]->url;
            table[{static_cast<unsigned short>(i+1), 3}] = settings[i]->download_url;
        }

        const string xlsx_path = fmt::format("{}/{}.xlsx", get_config()->get_tmp_path(), context->user->id);

        write_xlsx(xlsx_path, {{"Настройки", table}});

        return get_bot()->send_message({
            .chat_id = context->chat->id,
            .text = "📩Отправь редактированную таблицу обратно",
            .reply_message_id = context->message->id,
            .filepath = xlsx_path,
            .content_type = DOCUMENT,
        });
    }

    ptrMessage YouTubeMediaHandler::handle_audio_settings(shared_ptr<BotHandlerContext> context) {
        const string xlsx_path = fmt::format("{}/{}.xlsx", get_config()->get_tmp_path(), context->user->id);
        get_bot()->download_file(context->message->file_download_id, xlsx_path);

        auto raw_data = read_xlsx(xlsx_path);

        if (raw_data.empty()) {
            return get_bot()->send_message({
                .chat_id = context->chat->id,
                .text = "Информация не найдена в таблице!",
                .reply_message_id = context->message->id,
            });
        }

        const auto& sheet = raw_data.begin()->second;
        vector<unique_ptr<YouTubeAudioSetting> > result;

        for (unsigned short i = 1; sheet.contains({i, 2}) && sheet.contains({i, 3}); i++) {
            result.emplace_back(make_unique<YouTubeAudioSetting>(
                context->user->id,
                sheet.at({i, 2}),
                (sheet.contains({i, 1}) ? sheet.at({i, 1}) : ""),
                sheet.at({i, 3})
            ));
        }

        get_repositories()->youtube_audio_setting->update_by_user_id(context->user->id, std::move(result));

        return get_bot()->send_message({
            .chat_id = context->chat->id,
            .text = fmt::format("🆕Данные обновлены")
        });
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