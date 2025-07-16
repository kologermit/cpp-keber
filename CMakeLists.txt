cmake_minimum_required(VERSION 3.15)
project(CPP-KEBER)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_SHARED_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH};./bot;./utils;.")

include(./build/Release/generators/conan_toolchain.cmake)

include_directories(
    .
    ./bot
    ./test_code
)

find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Sqlpp11 REQUIRED)
find_package(spdlog REQUIRED)
find_package(libuuid REQUIRED)
find_package(httplib REQUIRED)

set(ALL_LINK_LIBRARIES 
    libuuid::libuuid
    sqlpp11::sqlpp11
    spdlog::spdlog
    fmt::fmt
    httplib::httplib
    nlohmann_json::nlohmann_json
)

set(
    UTILS_RELEASES
    ./utils/TGBotApi/Bot/Bot.cpp
    ./utils/TGBotApi/Chat/Chat.cpp
    ./utils/TGBotApi/File/File.cpp
    ./utils/TGBotApi/Message/Message.cpp
    ./utils/TGBotApi/Query/Query.cpp
    ./utils/TGBotApi/User/User.cpp
    ./utils/Random/Random.cpp
    ./utils/UUID4/UUID4.cpp
    ./utils/Logger/InterfaceLogger.cpp
    ./utils/Logger/StdOutLogger.cpp
    ./utils/Logger/FileDailyLogger.cpp
    ./utils/Env/Env.cpp
)

add_library(UTILS_LIB STATIC ${UTILS_RELEASES})
target_link_libraries(UTILS_LIB ${ALL_LINK_LIBRARIES})
target_include_directories(UTILS_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


option(DEBUG_ARGS "Build debug" OFF)
if (DEBUG_ARGS) 
    add_compile_definitions(DEBUG_LOGGER=1)
    add_compile_options(-g3 -O0)
endif()

option(BUILD_BOT "Build bot" OFF)
if (BUILD_BOT)
    add_executable(
        BOT 
        ./bot/main.cpp
    )
    target_link_libraries(
        BOT
        UTILS_LIB
        ${ALL_LINK_LIBRARIES}
    )
endif()

option(BUILD_TEST_CODE "Build test" OFF)
if (BUILD_TEST_CODE)
    add_executable(
        TEST_CODE 
        ./test_code/test_code.cpp
    )
    target_link_libraries(
        TEST_CODE
        UTILS_LIB
        ${ALL_LINK_LIBRARIES}
    )
endif()