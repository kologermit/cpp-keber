#pragma once

namespace Bot::BotHandler {

    constexpr const char* MENU_COMMAND = "/menu";
    constexpr const char* START_COMMAND = "/start";
    constexpr const char* D_COMMAND = "/d";
    constexpr const char* DICE_COMMAND = "/dice";

    constexpr const char* SUCCESS_SYMBOL = "✅";
    constexpr const char* ERROR_SYMBOL = "❌";

    constexpr const char* MENU_WORD = "🏠Меню";
    constexpr const char* BACK_WORD = "🔙Назад";
    constexpr const char* ACCESS_WORD = "🔑Доступ";
    constexpr const char* BASE_WORD = "🔰Базовые";
    constexpr const char* FULL_WORD = "👑Полные";
    constexpr const char* YOUTUBE_WORD = "🎥Ютуб";
    constexpr const char* TASK_WORD = "🎯Задачи";
    constexpr const char* DOCKER_WORD = "🐋Докер";
    constexpr const char* SERVER_WORD = "🖥️Сервер";
    constexpr const char* ADD_WORD = "➕Добавить";
    constexpr const char* DELETE_WORD = "❌Удалить";
    constexpr const char* AUDIO_WORD = "🔊Аудио";
    constexpr const char* VIDEO_WORD = "🎬Видео";
    constexpr const char* AUDIO_PLAYLIST_WORD = "▶️🔊Аудио-Плейлист";
    constexpr const char* VIDEO_PLAYLIST_WORD = "▶️🎬Видео-Плейлист";

    constexpr const char* NOT_ANDERSTAND_PHRASE = "Не понял!";
    constexpr const char* USER_NOT_FOUND_PHRASE = "Пользователь {} не найден!";
    constexpr const char* IN_DEVELOP_PHRASE = "Находится в разработке";
    constexpr const char* START_PHRASE = "Приветствую в CPP-KEBER!";
    constexpr const char* DICE_PHRASE = "Бросок кубика на <b>{}</b>\nРезультат: <b>{}</b>";
    constexpr const char* ACCESS_PHRASE = "<b>Ваши права:</b> <code>{}</code>\n\n<i>Отправь тег или id пользователя</i>";
    constexpr const char* ADD_ACCESS_PHRASE = "<b>Добавлен доступ для {} пользователем</b> <code>{}</code>";
    constexpr const char* REMOVE_ACCESS_PHRASE = "<b>Удалён доступ для {} пользователем</b> <code>{}</code>";
    constexpr const char* SEND_MEDIA_URL_PHRASE = "<b>Отправь одну или несколько ссылок {}, чтобы поставить на скачивание\n\nОдна строка - одна ссылка</b>";
    constexpr const char* MEDIA_NOT_FOUND_PHRASE = "<b>{} не найдено (<code>{}</code>)!</b>";
    constexpr const char* DOWNLOAD_MEDIA_PHRASE = "<b>Добавить {} ({}) в очередь скачивания?</b>\n{}";

    constexpr const char* USER_TEMPLATE = "<b>Имя:</b> <code>{}</code>\n<b>ID:</b> <code>{}</code>\n<b>TG ID:</b> <code>{}</code>\n<b>Тег:</b> <code>{}</code>";
    constexpr const char* MEDIA_TEMPLATE = "<b>{} | {}</b>\n<code>{}</code>";

}