cmake_minimum_required(VERSION 3.15)

project(TEST_CODE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_SHARED_LINKER_FLAGS "-static -static-libgcc -static-libstdc++ -Wl,--no-warn-rwx-segments")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -Wl,--as-needed -Wl,--no-warn-rwx-segments")
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

add_compile_options(
    -std=c++20 
    -Wall 
    -Wextra 
    -pedantic 
    -Werror 
    -Wconversion
    -Wno-error=deprecated-declarations
    -Wno-unused-command-line-argument
)

file(GLOB_RECURSE BOT_SOURCES
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(TEST_CODE ${BOT_SOURCES})

target_include_directories(TEST_CODE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_definitions(TEST_CODE PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

target_link_libraries(TEST_CODE
    libuuid::libuuid
    sqlpp11::sqlpp11
    fmt::fmt
    httplib::httplib
    nlohmann_json::nlohmann_json
    openssl::openssl
    datetime::datetime
    PostgreSQL::PostgreSQL
    pthread
    dl
    resolv
)