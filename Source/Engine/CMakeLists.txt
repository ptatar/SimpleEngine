cmake_minimum_required(VERSION 3.7)

if(${PLATFORM_WINDOWS})
    set(ENGINE_EXE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/main32.cpp
    )
elseif(${PLATFORM_LINUX})
    set(ENGINE_EXE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/mainX.cpp
    )
else()
    message(FATAL_ERROR "Unknown platform")
endif()

set(RENDERER_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Renderer)
set(COMMON_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Common)
set(WINDOW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Window)
set(THREAD_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Thread)
set(FILE_MANAGER_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/FileManager)

include_library(${COMMON_LIB_DIR} COMMON_LIB_INC_DIR)
include_library(${THREAD_LIB_DIR} THREAD_LIB_INC_DIR)
include_library(${WINDOW_LIB_DIR} WINDOW_LIB_INC_DIR)
include_library(${FILE_MANAGER_LIB_DIR} FILE_MANAGER_LIB_INC_DIR)
include_library(${RENDERER_LIB_DIR} RENDERER_LIB_INC_DIR)

add_executable(ENGINE_EXE ${ENGINE_EXE_SRC})
target_link_libraries(ENGINE_EXE WINDOW_LIB
                                 RENDERER_LIB
                                 THREAD_LIB)

target_include_directories(ENGINE_EXE PRIVATE ${WINDOW_LIB_INC_DIR}
                                      PRIVATE ${COMMON_LIB_INC_DIR}
                                      PRIVATE ${FILE_MANAGER_LIB_INC_DIR}
                                      PRIVATE ${RENDERER_LIB_INC_DIR}
                                      PRIVATE ${THREAD_LIB_INC_DIR}
                                      PRIVATE ${Vulkan_INCLUDE_DIRS})

if(${PLATFORM_WINDOWS})
    set_target_properties(ENGINE_EXE
        PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
