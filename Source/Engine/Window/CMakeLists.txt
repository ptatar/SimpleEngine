set(WINDOW_LIB_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(WINDOW_LIB_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set(LIBRARY_INC_DIR ${WINDOW_LIB_INC_DIR} PARENT_SCOPE)

set(WINDOW_LIB_SRC
    ${WINDOW_LIB_INC_DIR}/IWindowEventHandler.hpp
    ${WINDOW_LIB_INC_DIR}/IWindow.hpp
    ${WINDOW_LIB_SRC_DIR}/WindowManager.cpp
    ${WINDOW_LIB_INC_DIR}/WindowManager.hpp
)


if(${PLATFORM_WINDOWS})
    message("Windows")
    set(WINDOW_LIB_SRC ${WINDOW_LIB_SRC}
        ${WINDOW_LIB_SRC_DIR}/Window32.cpp
        ${WINDOW_LIB_INC_DIR}/Window32.hpp
    )
elseif(${PLATFORM_LINUX})
    message("LINUX")
    set(WINDOW_LIB_SRC ${WINDOW_LIB_SRC}
        ${WINDOW_LIB_SRC_DIR}/WindowX.cpp
        ${WINDOW_LIB_INC_DIR}/WindowX.hpp
    )
else()
    message(FATAL_ERROR "Unknown platform")
endif()

add_library(WINDOW_LIB STATIC ${WINDOW_LIB_SRC})

target_link_libraries(WINDOW_LIB PRIVATE COMMON_LIB
                                 PRIVATE RENDERER_LIB)

target_include_directories(WINDOW_LIB PRIVATE ${COMMON_LIB_INC_DIR}
                                      PRIVATE ${RENDERER_LIB_INC_DIR}
                                      PRIVATE ${WINDOW_LIB_INC_DIR})

if(${PLATFORM_LINUX})
    target_link_libraries(WINDOW_LIB PRIVATE ${X11_LIBRARIES})
    target_include_directories(WINDOW_LIB PRIVATE ${X11_INCLUDE_DIR})
endif()

