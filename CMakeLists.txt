cmake_minimum_required(VERSION 3.15)
project(KEBER_BOT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
set(CMAKE_SHARED_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

set(GENERATORS_DIR "./build/Release/generators")
set(libpqxx_DIR ${GENERATORS_DIR})
set(PostgreSQL_DIR ${GENERATORS_DIR})
set(asio_DIR ${GENERATORS_DIR})
set(libpqxx_DIR ${GENERATORS_DIR})
set(fmt_DIR ${GENERATORS_DIR})
set(Crow_DIR ${GENERATORS_DIR})
set(hiredis_DIR ${GENERATORS_DIR})

find_package(libpqxx REQUIRED)
find_package(fmt REQUIRED)
find_package(Crow REQUIRED)
find_package(hiredis REQUIRED)

add_executable(KEBER_BOT ./bot/main.cpp)
target_link_libraries(KEBER_BOT
    libpqxx::pqxx
    fmt::fmt
    Crow::Crow
    hiredis::hiredis
)
