cmake_minimum_required(VERSION 3.15)

project(BOT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE BOT_SOURCES
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(BOT ${BOT_SOURCES})

target_include_directories(BOT PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_definitions(BOT PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

target_link_libraries(BOT
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