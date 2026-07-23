#include <bot/BotHandler/GetStatistic/GetStatisticHandler.hpp>
#include <bot/BotHandler/TaskTracker/TaskTrackerHandler.hpp>
#include <pystring.h>
#include <fmt/format.h>

namespace Bot::BotHandler::GetStatistic {

    using Bot::BotHandler::TaskTracker::TaskTrackerHandler;
    using Utils::TaskTrackerApi::Task;
    using Utils::Datetime::datetime;
    using Utils::Datetime::DATE_FORMAT;
    using pystring::split;
    using std::exception;
    using std::vector;
    using std::stoll;
    using std::unique_ptr;

    const string& GetStatisticHandler::get_name() const noexcept {
        static const string name = "GetStatisticHandler";
        return name;
    }

    bool GetStatisticHandler::check(ptrContext ctx) {
        // Пример /get_statistic date
        return ctx->message->text.starts_with("/get_statistic ");
    }

    ptrMessage GetStatisticHandler::handle(ptrContext ctx) {
        vector<string> words = split(ctx->message->text, " ");
        datetime date;
        try {
            date = datetime::parse(DATE_FORMAT, words[1]);
        } catch (const exception& exc) {
            return ctx->bot->send_message({
                .chat_id = ctx->chat->id,
                .text = fmt::format("Не получилось определить дату {}. Ошибка {}", words[1], exc.what()),
                .reply_message_id = ctx->message->id,
            });
        }

        return TaskTrackerHandler::send_statistic(ctx, date);
    }
}