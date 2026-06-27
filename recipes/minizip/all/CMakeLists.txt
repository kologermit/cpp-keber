cmake_minimum_required(VERSION 3.15)
project(minizip LANGUAGES C)

option(MINIZIP_ENABLE_BZIP2 "Build minizip with bzip2 support" ON)
option(MINIZIP_BUILD_TOOLS "Build minizip tool" OFF)

include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

find_package(ZLIB REQUIRED)
if(MINIZIP_ENABLE_BZIP2)
    find_package(BZip2 REQUIRED)
endif()

set(SOURCE_FILES
    ${MINIZIP_SRC_DIR}/ioapi.c
    ${MINIZIP_SRC_DIR}/unzip.c
    ${MINIZIP_SRC_DIR}/zip.c
    ${MINIZIP_SRC_DIR}/mztools.c
)
if(WIN32)
    list(APPEND SOURCE_FILES ${MINIZIP_SRC_DIR}/iowin32.c)
endif()

set(HEADER_FILES
    ${MINIZIP_SRC_DIR}/ioapi.h
    ${MINIZIP_SRC_DIR}/unzip.h
    ${MINIZIP_SRC_DIR}/zip.h
    ${MINIZIP_SRC_DIR}/mztools.h
    ${MINIZIP_SRC_DIR}/crypt.h
)
if(WIN32)
    list(APPEND HEADER_FILES ${MINIZIP_SRC_DIR}/iowin32.h)
endif()

if(EXISTS "${MINIZIP_SRC_DIR}/ints.h")
    list(APPEND HEADER_FILES "${MINIZIP_SRC_DIR}/ints.h")
endif()

add_library(minizip ${SOURCE_FILES})
target_link_libraries(minizip PUBLIC ZLIB::ZLIB)
target_compile_definitions(minizip PRIVATE -D_ZLIB_H)
target_include_directories(minizip PUBLIC ${MINIZIP_SRC_DIR})
set_target_properties(minizip PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(MINIZIP_ENABLE_BZIP2)
    target_compile_definitions(minizip PUBLIC HAVE_BZIP2=1)
    target_link_libraries(minizip PUBLIC BZip2::BZip2)
endif()

install(
    TARGETS minizip
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/minizip)