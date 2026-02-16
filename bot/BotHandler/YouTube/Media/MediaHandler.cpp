#include <bot/BotHandler/YouTube/Media/MediaHandler.hpp>
#include <bot/BotHandler/YouTube/YouTubeHandler.hpp>
#include <bot/BotHandler/Keys.hpp>
#include <bot/Config/InterfaceConfig.hpp>
#include <utils/TGBotApi/Types.hpp>
#include <utils/YouTubeApi/InterfaceYouTubeApi.hpp>
#include <utils/XLSX/XLSX.hpp>
#include <fmt/core.h>
#include <sstream>

namespace Bot::BotHandler::YouTube::Media {
    using Bot::Entity::User::User;
    using Bot::Entity::User::UserScreen;
    using Bot::Entity::YouTubeAudioSetting::YouTubeAudioSetting;
    using Utils::TGBotApi::Types::ReplyKeyboard;
    using Utils::TGBotApi::Types::ReplyButtons;
    using Utils::TGBotApi::Types::ReplyButton;
    using Utils::TGBotApi::Types::InlineKeyboard;
    using Utils::TGBotApi::Types::InlineButtons;
    using Utils::TGBotApi::Types::InlineButton;
    using Utils::TGBotApi::File::DOCUMENT;
    using Utils::XLSX::read_xlsx;
    using Utils::XLSX::write_xlsx;
    using Utils::XLSX::CellCoords;
    using nlohmann::json;
    using std::to_string;
    using std::map;
    using std::vector;
    using std::unique_ptr;
    using std::make_unique;
    using std::make_shared;
    using std::istringstream;
    using std::getline;

    ptrMessage MediaHandler::to_youtube_media(shared_ptr<BotHandlerContext> ctx, bool is_video) {
        static const string VIDEO_TEXT = fmt::format(fmt::runtime(SEND_MEDIA_URL_PHRASE), VIDEO_WORD);
        static const string AUDIO_TEXT = fmt::format(fmt::runtime(SEND_MEDIA_URL_PHRASE), AUDIO_WORD);
        static const ReplyButtons VIDEO_KEYBOARD{{make_shared<ReplyButton>(BACK_WORD)}};
        static const ReplyButtons AUDIO_KEYBOARD{{make_shared<ReplyButton>(SETTINGS_TABLE_WORD), make_shared<ReplyButton>(BACK_WORD)}};

        ctx->user->screen = (is_video ? UserScreen::YOUTUBE_VIDEO : UserScreen::YOUTUBE_AUDIO);
        ctx->db->user->update(*ctx->user);
        return ctx->bot->send_message( {
            .chat_id = ctx->chat->id,
            .text = (is_video ? VIDEO_TEXT : AUDIO_TEXT),
            .reply_message_id = ctx->message->id,
            .reply_keyboard = make_unique<ReplyKeyboard>(is_video ? VIDEO_KEYBOARD : AUDIO_KEYBOARD)
        });
    }

    const string& MediaHandler::get_name() const noexcept {
        static const string name = "YouTubeMediaHandler";
        return name;
    }

    bool MediaHandler::check(shared_ptr<BotHandlerContext> ctx) {
        return (
            ctx->access.full
            || ctx->access.youtube
        ) && (
            ctx->user->screen == UserScreen::YOUTUBE_VIDEO
            || ctx->user->screen == UserScreen::YOUTUBE_AUDIO
        ) && (
            ctx->message->text.starts_with("https")
            || ctx->message->text == BACK_WORD
            || ctx->message->text == SETTINGS_TABLE_WORD
            || ctx->message->file_name.ends_with(".xlsx")
        );
    }

    ptrMessage MediaHandler::handle(shared_ptr<BotHandlerContext> ctx) {
        if (ctx->message->text == BACK_WORD) {
            return YouTubeHandler::to_youtube(ctx);
        }
        if (ctx->user->screen == UserScreen::YOUTUBE_AUDIO && ctx->message->text == SETTINGS_TABLE_WORD) {
            return send_audio_settings(ctx);
        }
        if (ctx->user->screen == UserScreen::YOUTUBE_AUDIO && ctx->message->file_name.ends_with(".xlsx")) {
            return handle_audio_settings(ctx);
        }
        return handle_urls(ctx);
    }

    ptrMessage MediaHandler::send_audio_settings(shared_ptr<BotHandlerContext> ctx) {
        auto settings = ctx->db->youtube_audio_setting->get_by_user_id(ctx->user->id);

        map<CellCoords, string> table;

        table[{0, 0}] = "# (необязательно)";
        table[{0, 1}] = "Название файл (необязательно)";
        table[{0, 2}] = "Ссылка Ютуб";
        table[{0, 3}] = "Ссылка для скачивания";

        for (size_t i = 0; i < settings->size(); i++) {
            auto row = static_cast<unsigned short>(i+1);
            table[{row, 0}] = to_string(i+1);
            table[{row, 1}] = settings->at(i).file_name;
            table[{row, 2}] = settings->at(i).url;
            table[{row, 3}] = settings->at(i).download_url;
        }

        const string xlsx_path = fmt::format("{}/{}.xlsx", ctx->config->get_tmp_path(), ctx->user->id);

        write_xlsx(xlsx_path, {{"Настройки", table}});

        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = "✏️Заполни и отправь обратно",
            .reply_message_id = ctx->message->id,
            .filepath = xlsx_path,
            .content_type = DOCUMENT,
        });
    }

    ptrMessage MediaHandler::handle_audio_settings(shared_ptr<BotHandlerContext> ctx) {
        const string xlsx_path = fmt::format("{}/{}.xlsx", ctx->config->get_tmp_path(), ctx->user->id);
        ctx->bot->download_file(ctx->message->file_download_id, xlsx_path);

        auto raw_data = read_xlsx(xlsx_path);

        if (raw_data.empty()) {
            return ctx->bot->send_message({
                .chat_id = ctx->chat->id,
                .text = "Информация не найдена в таблице!",
                .reply_message_id = ctx->message->id,
            });
        }

        const auto& sheet = raw_data.begin()->second;
        vector<YouTubeAudioSetting> result;

        for (unsigned short i = 1; sheet.contains({i, 2}) && sheet.contains({i, 3}); i++) {
            string url = sheet.at({i, 2});
            const string download_url = sheet.at({i, 3});
            const string file_name = sheet.contains({i, 1}) ? sheet.at({i, 1}) : "";

            auto video_data = ctx->global_ctx->api->youtube->get_video(url);
            
            if (video_data == nullptr) {
                return ctx->bot->send_message({
                    .chat_id = ctx->chat->id,
                    .text = fmt::format("Неверная ютуб-ссылка {}", url)
                });
            }

            url = video_data->first.video_url;

            if (!download_url.starts_with("http")) {
                return ctx->bot->send_message({
                    .chat_id = ctx->chat->id,
                    .text = fmt::format("Неверная ссылка для скачивания {} (должна начинаться с http или https)", download_url)
                });
            }

            result.emplace_back(
                ctx->user->id,
                url,
                file_name,
                download_url
            );
        }

        ctx->db->youtube_audio_setting->update_by_user_id(ctx->user->id, result);

        return ctx->bot->send_message({
            .chat_id = ctx->chat->id,
            .text = fmt::format("🆕Данные обновлены")
        });
    }

    ptrMessage MediaHandler::handle_urls(shared_ptr<BotHandlerContext> ctx) {
        const char* MEDIA_TYPE_WORD = (
            ctx->user->screen == UserScreen::YOUTUBE_VIDEO
            ? VIDEO_WORD
            : AUDIO_WORD
        );
        istringstream stream(ctx->message->text);
        string buffer, answer;
        int count = 0;
        while (getline(stream, buffer)) {
            if (buffer.empty()) {
                continue;
            }
            auto result = ctx->global_ctx->api->youtube->get_video(buffer);
            if (result == nullptr) {
                return ctx->bot->send_message( {
                    .chat_id = ctx->chat->id,
                    .text = format(MEDIA_NOT_FOUND_PHRASE, MEDIA_TYPE_WORD, buffer),
                    .reply_message_id = ctx->message->id,
                });
            }
            const auto& [video, channel] = *result;
            answer += "\n\n" +  fmt::format(MEDIA_TEMPLATE, video.title, channel.title, video.video_url);
            count++;
        }

        return ctx->bot->send_message( {
            .chat_id = ctx->chat->id,
            .text = format(DOWNLOAD_MEDIA_PHRASE,
                MEDIA_TYPE_WORD,
                count,
                answer
            ),
            .reply_message_id = ctx->message->id,
            .inline_keyboard = make_unique<InlineKeyboard>(
                ctx->user->screen == UserScreen::YOUTUBE_VIDEO
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