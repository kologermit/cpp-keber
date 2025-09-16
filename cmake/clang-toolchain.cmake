set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_C_FLAGS_INIT "-march=native")
set(CMAKE_CXX_FLAGS_INIT "-march=native")

set(CMAKE_C_FLAGS_RELEASE_INIT "-O3 -flto=thin")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -flto=thin")

message(STATUS "Using Clang compiler: ${CMAKE_CXX_COMPILER}")