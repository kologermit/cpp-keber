cmake_minimum_required(VERSION 3.15)

project(UTILS LANGUAGES CXX)

file(GLOB_RECURSE UTILS_SOURCES 
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

if(UNITY_BUILD AND UTILS_SOURCES)
    message(STATUS "Creating unity build for UTILS_LIB")
    create_unity_build_target(UTILS_LIB UTILS_SOURCES "utils_unity.cpp")
else()
    message(STATUS "Using normal build for UTILS_LIB")
endif()

add_library(UTILS_LIB STATIC ${UTILS_SOURCES})
target_include_directories(UTILS_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_definitions(UTILS_LIB PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

target_link_libraries(UTILS_LIB
    PUBLIC
    datetime::datetime
    libuuid::libuuid
    libpqxx::pqxx
    fmt::fmt
    httplib::httplib
    nlohmann_json::nlohmann_json
    openssl::openssl
    pthread
    dl
    resolv
)
