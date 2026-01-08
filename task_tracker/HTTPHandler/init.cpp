#include <task_tracker/HTTPHandler/init.hpp>
#include <task_tracker/HTTPHandler/Health/GETHealthHandler.hpp>

namespace TaskTracker::HTTPHandler {
    using std::vector;
    using std::make_shared;
    using TaskTracker::HTTPHandler::Health::GETHealthHandler;

    void init_server(IServer& server) {
        server.add_handlers(vector<IServer::ptrIHandler>{
            make_shared<GETHealthHandler>(),
        });
    }
}