cmake_minimum_required(VERSION 3.15)

project(UTILS LANGUAGES CXX)

file(GLOB_RECURSE UTILS_SOURCES 
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(UTILS_LIB STATIC ${UTILS_SOURCES})
target_include_directories(UTILS_LIB PRIVATE ${DEPS_PATHS} .)
target_compile_definitions(UTILS_LIB PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

# target_link_libraries(UTILS_LIB PRIVATE )