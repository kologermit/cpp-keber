#pragma once

#include <bot/BotHandler/InterfaceBotHandler.hpp>
#include <utils/TaskTrackerApi/Task.hpp>
#include <vector>
#include <optional>

namespace Bot::BotHandler::TaskTracker {
    using std::string_view;
    using std::vector;
    using std::optional;
    using std::nullopt;
    using Utils::TaskTrackerApi::Task;

    constexpr const char* TASK_TRACKER_CALLBACK_HANDLER_NAME = "ttch";

    struct TaskTrackerHandler : InterfaceBotHandler {
        const string& get_name() const noexcept override;
        bool check(ptrContext ctx) override;
        ptrMessage handle(ptrContext ctx) override;
        static ptrMessage to_task_tracker(ptrContext ctx, string_view message_text = "");
        static ptrMessage send_statistic(ptrContext ctx);
        static ptrMessage send_tasks(ptrContext ctx, optional<vector<Task> > tasks=nullopt);
    };
}