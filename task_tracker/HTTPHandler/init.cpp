#include <task_tracker/HTTPHandler/init.hpp>
#include <task_tracker/HTTPHandler/Health/GETHealthHandler.hpp>
#include <task_tracker/HTTPHandler/Task/POSTTaskHandler.hpp>
#include <task_tracker/HTTPHandler/Task/PATCHTaskHandler.hpp>
#include <task_tracker/HTTPHandler/Task/GETTaskHandler.hpp>

namespace TaskTracker::HTTPHandler {
    using std::vector;
    using std::make_shared;
    using TaskTracker::HTTPHandler::Health::GETHealthHandler;
    using TaskTracker::HTTPHandler::Task::POSTTaskHandler;
    using TaskTracker::HTTPHandler::Task::PATCHTaskHandler;
    using TaskTracker::HTTPHandler::Task::GETTaskHandler;

    void init_server(IServer& server) {
        server.add_handlers(vector<IServer::ptrIHandler>{
            make_shared<POSTTaskHandler>(),
            make_shared<PATCHTaskHandler>(),
            make_shared<GETTaskHandler>(),
            make_shared<GETHealthHandler>(),
        });
    }
}