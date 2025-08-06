cmake_minimum_required(VERSION 3.15)

project(UTILS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE UTILS_SOURCES 
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(UTILS_LIB STATIC ${UTILS_SOURCES})
target_include_directories(UTILS_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_definitions(UTILS_LIB PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

target_link_libraries(UTILS_LIB
    PRIVATE
    datetime::datetime
    libuuid::libuuid
    libpqxx::pqxx
    spdlog::spdlog
    fmt::fmt
    httplib::httplib
    nlohmann_json::nlohmann_json
    openssl::openssl
    pthread
    dl
    resolv
)