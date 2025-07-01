cmake_minimum_required(VERSION 3.15)
project(BOT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_SHARED_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

include(./build/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)

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
find_package(spdlog REQUIRED)

set(ALL_LINK_LIBRARIES 
    spdlog::spdlog 
    fmt::fmt 
    Crow::Crow 
    hiredis::hiredis 
    sqlpp11::sqlpp11
)

option(BUILD_BOT "Build bot" OFF)
if (BUILD_BOT)
    add_executable(BOT ./bot/main.cpp)
    target_link_libraries(BOT
        ${ALL_LINK_LIBRARIES}
    )
endif()

option(BUILD_TEST_CODE "Build test" OFF)
if (BUILD_TEST_CODE)
    add_executable(TEST_CODE ./test_code/test_code.cpp)
    target_link_libraries(TEST_CODE
        ${ALL_LINK_LIBRARIES}
    )
endif()