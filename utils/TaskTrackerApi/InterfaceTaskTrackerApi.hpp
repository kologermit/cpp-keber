#pragma once

#include <utils/TaskTrackerApi/Task.hpp>
#include <memory>

namespace Utils::TaskTracker {
    using std::unique_ptr;

    struct InterfaceTaskTrackerApi {
        virtual ~InterfaceTaskTrackerApi() = default;

        virtual unique_ptr<Task> post_task(

        ) = 0;
    };
}