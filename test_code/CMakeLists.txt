cmake_minimum_required(VERSION 3.15)

project(BOT LANGUAGES CXX)

file(GLOB_RECURSE BOT_SOURCES
        LIST_DIRECTORIES false
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(BOT ${BOT_SOURCES})

target_include_directories(BOT PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_definitions(TEST_CODE PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

target_link_libraries(TEST_CODE
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