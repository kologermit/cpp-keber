cmake_minimum_required(VERSION 3.15)

project(CPP-KEBER CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 20)

add_compile_options(
    -Wall 
    -Wextra 
    -pedantic 
    -Werror
    -Wconversion
    -Wno-error=deprecated-declarations
    -Wno-unused-command-line-argument
    -DCPPHTTPLIB_OPENSSL_SUPPORT
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g3 -O0 -D_GLIBCXX_DEBUG)
    message(STATUS "Debug mode enabled")
else()
    add_compile_options(-O3 -DNDEBUG -flto=auto)
    message(STATUS "Release mode enabled")
endif()

find_package(PostgreSQL REQUIRED)

set(DEPS_STATICS 
    ${CMAKE_SOURCE_DIR}/build/_deps/libuuid/build/libuuid_static.a
    ${CMAKE_SOURCE_DIR}/build/_deps/libpqxx/build/src/libpqxx-7.10.a
    ${CMAKE_SOURCE_DIR}/build/_deps/fmt/build/libfmt.a
    ${CMAKE_SOURCE_DIR}/build/_deps/datetime/build/libdatetime.a
    ${CMAKE_SOURCE_DIR}/build/_deps/xlsxio/build/libxlsxio_read.a
    ${CMAKE_SOURCE_DIR}/build/_deps/xlsxio/build/libxlsxio_write.a
    ${CMAKE_SOURCE_DIR}/build/_deps/libxlsxwriter/build/libxlsxwriter.a
    ${CMAKE_SOURCE_DIR}/build/_deps/openssl/libssl.a
    ${CMAKE_SOURCE_DIR}/build/_deps/openssl/libcrypto.a
    ${CMAKE_SOURCE_DIR}/build/_deps/libexpat/expat/build/libexpat.a
    ${CMAKE_SOURCE_DIR}/build/_deps/minizip-ng/build/libminizip.a
    ${CMAKE_SOURCE_DIR}/build/_deps/zlib-ng/build/libz.a
    PostgreSQL::PostgreSQL
)

set(DEPS_PATHS 
    ${CMAKE_SOURCE_DIR}/build/_deps/libuuid/build/include
    ${CMAKE_SOURCE_DIR}/build/_deps/cpp-httplib
    ${CMAKE_SOURCE_DIR}/build/_deps/libpqxx/include
    ${CMAKE_SOURCE_DIR}/build/_deps/libpqxx/build/include
    ${CMAKE_SOURCE_DIR}/build/_deps/nlohmann-json/single_include
    ${CMAKE_SOURCE_DIR}/build/_deps/fmt/include
    ${CMAKE_SOURCE_DIR}/build/_deps/datetime/src
    ${CMAKE_SOURCE_DIR}/build/_deps/xlsxio/include/
    ${CMAKE_SOURCE_DIR}/build/_deps/libxlsxwriter/include
    ${CMAKE_SOURCE_DIR}
)

add_subdirectory(utils)

option(BUILD_TEST_CODE "Build test code" OFF)
if (BUILD_TEST_CODE)
    file(GLOB_RECURSE TEST_SOURCES
        LIST_DIRECTORIES false
        "./test_code/*.cpp"
    )
    add_executable(TEST_CODE ${TEST_SOURCES})
    target_include_directories(TEST_CODE PRIVATE ${DEPS_PATHS})
    set_target_properties(BOT PROPERTIES
        UNITY_BUILD ON
        UNITY_BUILD_BATCH_SIZE 20
    )
    target_link_libraries(TEST_CODE PRIVATE
        UTILS_LIB
        ${DEPS_STATICS}
    )
endif()

option(BUILD_BOT "Build bot" OFF)
if (BUILD_BOT)
    file(GLOB_RECURSE BOT_SOURCES
        LIST_DIRECTORIES false
        "./bot/*.cpp"
    )
    add_executable(BOT ${BOT_SOURCES})
    target_include_directories(BOT PRIVATE 
        ${DEPS_PATHS}
    )
    target_link_libraries(BOT PRIVATE
        UTILS_LIB
        ${DEPS_STATICS}
    )
endif()

option(BUILD_TASK_TRACKER "Build task tracker" OFF)
if (BUILD_TASK_TRACKER)
    file(GLOB_RECURSE TASK_TRACKER_SOURCES
        LIST_DIRECTORIES false
        "./task_tracker/*.cpp"
    )
    add_executable(TASK_TRACKER ${TASK_TRACKER_SOURCES})
    target_include_directories(TASK_TRACKER PRIVATE 
        ${DEPS_PATHS}
    )
    target_link_libraries(TASK_TRACKER PRIVATE
        UTILS_LIB
        ${DEPS_STATICS}
    )
endif()