#include <bot/BotHandler/Help/HelpHandler.hpp>
#include <fmt/format.h>
#include <pystring.h>
#include <vector>

namespace Bot::BotHandler::Help {

    using Utils::TGBotApi::Bot::SendMessageParameters;
    using Utils::TGBotApi::Types::ptrMessage;
    using pystring::join;
    using std::string;
    using std::vector;

    const string& HelpHandler::get_name() const noexcept {
        static const string name = "HelpHandler";
        return name;
    }

    bool HelpHandler::check(ptrContext ctx) {
        // Пример /get_tasks date state1,state2...
        // date - дата в формате YYYY-MM-DD, по умолчанию текущая дата
        // state - состояние задачи, по умолчанию new, возможеные: completed, deleted, in_work, new
        return (ctx->access.full || ctx->access.base)
            && ctx->message->text.starts_with("/help");
    }

    ptrMessage HelpHandler::handle(ptrContext ctx) {
        static const string task_tracker_text = join("\n", {
            "<b>Команды для усправления задачами:</b>",
            "  <b>Просмотр задачи по id</b>",
            "  <code>/get_task {task_id}</code>",
            "",
            "  <b>Просмотр задач за день</b>",
            "  <code>/get_tasks {date} {states}</code>",
            "    <u>{date}</u> - <i>дата просмотра списка задач. чтобы пропустить параметр можно написать \"-\". по умолчанию сегодняшний день</i>",
            "    <u>{states}</u> - <i>список состояний задач (к примеру new,in_word) из new, in_work, completed, deleted. По умолчанию new</i>",
            "",
            "  <b>Создание задачи</b>",
            "  <code>/new_task {date}",
            "  {title}",
            "  {description}",
            "  ...</code>",
            "    <u>{date}</u> - <i>дача начала задачи. необязательный параметр. по умолчанию сегодняшний день</i>",
            "    <u>{title}</u> - <i>заголовок задачи. обязательный параметр</i>",
            "    <u>{description}</u> - <i>описание задачи. необзательный параметр. по умолчанию \"[Пусто]\"</i>",
            "",
            "  <b>Изменение задачи</b>"
            "  <code>/patch_task {task_id} {field}",
            "  {value}</code>",
            "    <u>{task_id}</u> - <i>номер задачи</i>",
            "    <u>{field}</u> - <i>изменяемое поле (title/description/start_at/state)</i>",
            "    <u>{value}</u> - <i>новое значение поля</i>",
            "",
            "  <b>Просмотр статистики за день</b>",
            "  <code>/get_statistic {date}</code>",
            "    <u>{date}</u> - <i>дата. необязательный параметр. по умолчанию сегодняшяя дата</i>"
        });
        static const string base_text = join("\n", {
            "<b>Базовые команды</b>",
            "  <b>Вернуться в меню</b>",
            "  <code>/menu</code> и <code>/start</code>",
            "",
            "  <b>Бросок кубика на заданное число. По умолчанию 20</b>",
            "  <code>/d</code>, <code>/d4</code>, <code>/d6</code>, <code>/d8</code>, <code>/d10</code>, <code>/d12</code>, <code>/d20</code>",
            "",
            "  <b>Просмотр помощи</b>"
            "  <code>/help</code>",
        });
        ptrMessage last_message;
        SendMessageParameters msg_params{.chat_id = ctx->chat->id, .reply_message_id = ctx->message->id};
        if (ctx->access.full || ctx->access.base) {
            msg_params.text = base_text;
            last_message = ctx->bot->send_message(msg_params);
        }
        if (ctx->access.full || ctx->access.task_tracker) {
            msg_params.text = task_tracker_text;
            last_message = ctx->bot->send_message(msg_params);
        }
        return last_message;
    }
}