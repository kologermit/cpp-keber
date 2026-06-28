cmake_minimum_required(VERSION 3.15)

project(TASK_TRACKER LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE TASK_TRACKER_SOURCES
        LIST_DIRECTORIES false
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(TASK_TRACKER ${TASK_TRACKER_SOURCES})

target_include_directories(TASK_TRACKER PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_definitions(TASK_TRACKER PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

target_link_libraries(TASK_TRACKER
        UTILS_LIB
        project_deps
)