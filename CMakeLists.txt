cmake_minimum_required(VERSION 3.15)

if(NOT COMPILER)
    set(COMPILER clang)
endif()

include(./cmake/${COMPILER}-toolchain.cmake)
include(./cmake/unity-build-toolchain.cmake)

project(CPP-KEBER CXX)

option(UNITY_BUILD "Enable unity build for faster compilation" ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH};./bot;./utils;.")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

include(./build/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)

add_compile_options(
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

find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(libuuid REQUIRED)
find_package(httplib REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(libpqxx REQUIRED)
find_package(datetime REQUIRED)
find_package(xlsxio REQUIRED)
find_package(libxlsxwriter REQUIRED)
find_package(GTest REQUIRED)

set(OPENSSL_USE_STATIC_LIBS TRUE)

add_subdirectory(utils)

option(BUILD_BOT "Build bot" OFF)
if (BUILD_BOT)
    add_subdirectory(bot)
    target_link_libraries(
        BOT
        UTILS_LIB
    )
endif()

option(BUILD_TASK_TRACKER "Build task_tracker" OFF)
if (BUILD_TASK_TRACKER)
    add_subdirectory(task_tracker)
    target_link_libraries(
            TASK_TRACKER
            UTILS_LIB
    )
endif()

option(BUILD_TEST_CODE "Build test" OFF)
if (BUILD_TEST_CODE)
    file(GLOB_RECURSE TEST_SOURCES
            LIST_DIRECTORIES false
            "./test_code/*.cpp"
    )

    add_executable(TEST_CODE ${TEST_SOURCES})

    target_include_directories(TEST_CODE PRIVATE
        .
        ./test_code
        ./bot
        ./utils
    )

    target_link_libraries(TEST_CODE
        openssl::openssl
        libpqxx::pqxx
        libuuid::libuuid
        datetime::datetime
        fmt::fmt
        httplib::httplib
        nlohmann_json::nlohmann_json
        xlsxio::xlsxio
        libxlsxwriter::libxlsxwriter
        gtest::gtest
        pthread
        dl
        resolv
    )

    target_link_libraries(
        TEST_CODE
        UTILS_LIB
    )
endif()