set(RENDERER_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Renderer)
set(COMMON_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Common)
set(WINDOW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Window)

set(COMMON_LIB_INC_DIR ${COMMON_LIB_DIR}/include)
set(WINDOW_LIB_INC_DIR ${WINDOW_LIB_DIR}/include)
set(RENDERER_LIB_INC_DIR ${RENDERER_LIB_DIR}/include)

if(${PLATFORM_WINDOWS})
    set(ENGINE_EXE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/main32.cpp
    )
    add_definitions(-D_UNICODE)
elseif(${PLATFORM_LINUX})
    set(ENGINE_EXE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/mainX.cpp
    )
else()
    message(FATAL_ERROR "Unknown platform")
endif()

add_subdirectory(${COMMON_LIB_DIR})
add_subdirectory(${RENDERER_LIB_DIR})
add_subdirectory(${WINDOW_LIB_DIR})

add_executable(ENGINE_EXE ${ENGINE_EXE_SRC})
target_link_libraries(ENGINE_EXE WINDOW_LIB)
target_include_directories(ENGINE_EXE PRIVATE ${WINDOW_LIB_INC_DIR})
target_include_directories(ENGINE_EXE PRIVATE ${COMMON_LIB_INC_DIR})

if(${PLATFORM_WINDOWS})
    set_target_properties(ENGINE_EXE
        PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
