cmake_minimum_required(VERSION 3.15)
project(BOT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_SHARED_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

include(./build/Release/generators/conan_toolchain.cmake)

include_directories(
    ".",
    "./utils",
    "./bot",
    "./downloader",
    "./cleaner",
    "./cron"
)

find_package(fmt REQUIRED)
find_package(Crow REQUIRED)
find_package(hiredis REQUIRED)
find_package(Sqlpp11 REQUIRED)

option(BUILD_BOT "Build bot" OFF)
if (BUILD_BOT)
    add_executable(BOT ./bot/main.cpp)
    target_link_libraries(BOT
        fmt::fmt
        Crow::Crow
        hiredis::hiredis
        sqlpp11::sqlpp11
    )
endif()