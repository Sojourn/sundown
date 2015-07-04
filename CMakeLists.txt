cmake_minimum_required(VERSION 2.8)

project(sundown)

if(UNIX)
    set(SUNDOWN_OS_POSIX TRUE)
    add_definitions(-DSUNDOWN_OS_POSIX)
elseif(WIN32)
    set(SUNDOWN_OS_WIN32 TRUE)
    add_definitions(-DSUNDOWN_OS_WIN32)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND CMAKE_CXX_FLAGS "-g -std=c++0x -lrt ${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src)

