cmake_minimum_required(VERSION 3.15)
project(CPP-KEBER CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_SHARED_LINKER_FLAGS "-static -static-libgcc -static-libstdc++ -Wl,--no-warn-rwx-segments")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -Wl,--as-needed -Wl,--no-warn-rwx-segments")
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH};./bot;./utils;.")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

include(./build/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)

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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g3 -O0 -D_GLIBCXX_DEBUG)
    message(STATUS "Debug mode enabled")
else()
    add_compile_options(-O3 -DNDEBUG -flto=auto)
    message(STATUS "Release mode enabled")
endif()

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
find_package(OpenSSL REQUIRED)

set(OPENSSL_USE_STATIC_LIBS TRUE)

set(ALL_LINK_LIBRARIES 
    libuuid::libuuid
    sqlpp11::sqlpp11
    spdlog::spdlog
    fmt::fmt
    httplib::httplib
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
    -pthread
    -ldl
    -lresolv
)

add_subdirectory(utils)

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