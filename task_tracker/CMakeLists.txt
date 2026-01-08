cmake_minimum_required(VERSION 3.15)

project(TASK_TRACKER LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE TASK_TRACKER_SOURCES
        LIST_DIRECTORIES false
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

if(UNITY_BUILD AND TASK_TRACKER_SOURCES)
    message(STATUS "Creating unity build for TASK_TRACKER")
    create_unity_build_target(TASK_TRACKER TASK_TRACKER_SOURCES "task_tracker_unity.cpp")
else()
    message(STATUS "Using normal build for TASK_TRACKER")
endif()

add_executable(TASK_TRACKER ${TASK_TRACKER_SOURCES})

target_include_directories(TASK_TRACKER PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_definitions(TASK_TRACKER PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

target_link_libraries(TASK_TRACKER
        UTILS_LIB
        openssl::openssl
        libpqxx::pqxx
        libuuid::libuuid
        datetime::datetime
        fmt::fmt
        httplib::httplib
        nlohmann_json::nlohmann_json
        pthread
        dl
        resolv
)