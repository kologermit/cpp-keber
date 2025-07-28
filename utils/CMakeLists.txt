cmake_minimum_required(VERSION 3.15)

project(UTILS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(UTILS_SOURCES
    Env/Env.cpp
    Logger/FileDailyLogger.cpp
    Logger/InterfaceLogger.cpp
    Logger/JSONSTDOutLogger.cpp
    Logger/StdOutLogger.cpp
    Random/Random.cpp
    TGBotApi/Bot/Bot.cpp
    TGBotApi/CallbackQuery/CallbackQuery.cpp
    TGBotApi/Chat/Chat.cpp
    TGBotApi/File/File.cpp
    TGBotApi/Message/Keyboard/InlineButton.cpp
    TGBotApi/Message/Keyboard/InlineKeyboard.cpp
    TGBotApi/Message/Keyboard/ReplyButton.cpp
    TGBotApi/Message/Keyboard/ReplyKeyboard.cpp
    TGBotApi/Message/Message.cpp
    TGBotApi/Query/Query.cpp
    TGBotApi/User/User.cpp
    UUID4/UUID4.cpp
)

list(TRANSFORM UTILS_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
add_library(UTILS_LIB STATIC ${UTILS_SOURCES})
target_include_directories(UTILS_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_definitions(UTILS_LIB PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

target_link_libraries(UTILS_LIB
    PRIVATE
    libuuid::libuuid
    sqlpp11::sqlpp11
    spdlog::spdlog
    fmt::fmt
    httplib::httplib
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
    pthread
    dl
    resolv
)