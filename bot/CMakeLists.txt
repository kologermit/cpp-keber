cmake_minimum_required(VERSION 3.15)
project(KEBER_BOT)

include_directories(.)

find_package(Poco REQUIRED)

add_executable(KEBER_BOT main.cpp)

target_link_libraries(KEBER_BOT Poco::Poco)