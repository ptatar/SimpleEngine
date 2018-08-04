set(WINDOW_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(WINDOW_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set(LIBRARY_INC_DIR ${WINDOW_INC_DIR} PARENT_SCOPE)

set(WINDOW_SRC
    ${WINDOW_INC_DIR}/IWindowEventHandler.hpp
    ${WINDOW_INC_DIR}/IWindow.hpp
    ${WINDOW_SRC_DIR}/WindowManager.cpp
    ${WINDOW_INC_DIR}/WindowManager.hpp
)


if(${PLATFORM_WINDOWS})
    message("Windows")
    set(WINDOW_SRC ${WINDOW_SRC}
        ${WINDOW_SRC_DIR}/Window32.cpp
        ${WINDOW_INC_DIR}/Window32.hpp
    )
elseif(${PLATFORM_LINUX})
    message("LINUX")
    set(WINDOW_SRC ${WINDOW_SRC}
        ${WINDOW_SRC_DIR}/WindowX.cpp
        ${WINDOW_INC_DIR}/WindowX.hpp
    )
else()
    message(FATAL_ERROR "Unknown platform")
endif()

add_library(WINDOW STATIC ${WINDOW_SRC})

target_link_libraries(WINDOW PRIVATE COMMON
                             PRIVATE RENDERER
                             PRIVATE THREAD)

target_include_directories(WINDOW PRIVATE ${COMMON_INC_DIR}
                                  PRIVATE ${RENDERER_INC_DIR}
                                  PRIVATE ${WINDOW_INC_DIR}
                                  PRIVATE ${THRED_INC_DIR})

if(${PLATFORM_LINUX})
    target_link_libraries(WINDOW PRIVATE ${X11_LIBRARIES})
    target_include_directories(WINDOW PRIVATE ${X11_INCLUDE_DIR})
endif()

