cmake_minimum_required(VERSION 3.7)
project(SimpleEngine)

set(SIMPLE_ENGINE_VERSION_MAJOR 0)
set(SIMPLE_ENGINE_VERSION_MINOR 1)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if (BUILD_TYPE STREQUAL "DEBUG")
    add_definitions(-DDEBUG_BUILD)
    message("Debug build")
elseif (BUILD_TYPE STREQUAL "RELEASE")
    add_definitions(-DRELEASE_BUILD -DUSE_COMMON_INTERFACE)
    message("Release build")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_FILES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Cmake")
include(platform)
include(compiler)
include(functions)

set(SOURCE_DIR "Source")
set(EXTERNAL_DIR "External")

add_subdirectory(${EXTERNAL_DIR})
add_subdirectory(${SOURCE_DIR})
