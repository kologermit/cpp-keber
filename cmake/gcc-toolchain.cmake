set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_C_FLAGS_INIT "-march=native -mtune=native")
set(CMAKE_CXX_FLAGS_INIT "-march=native -mtune=native")

set(CMAKE_C_FLAGS_RELEASE_INIT "-O3 -flto=auto -fuse-linker-plugin")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -flto=auto -fuse-linker-plugin")

message(STATUS "Using GCC compiler: ${CMAKE_CXX_COMPILER}")