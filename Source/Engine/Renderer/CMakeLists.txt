set(RENDERER_LIB_SRC_DIR ${RENDERER_LIB_DIR}/Source)
set(RENDERER_LIB_INC_DIR ${RENDERER_LIB_DIR}/Include)
set(LIBRARY_INC_DIR ${RENDERER_LIB_INC_DIR} PARENT_SCOPE)

if(${PLATFORM_WINDOWS})
    set(RENDERER_LIB_SRC
        ${RENDERER_LIB_SRC_DIR}/DeviceVk.cpp
        ${RENDERER_LIB_SRC_DIR}/RendererVk.cpp
        ${RENDERER_LIB_INC_DIR}/DeviceVk.hpp
        ${RENDERER_LIB_INC_DIR}/RendererVk.hpp
        ${RENDERER_LIB_INC_DIR}/IRenderer.hpp
        ${RENDERER_LIB_INC_DIR}/RendererManager.hpp
        ${RENDERER_LIB_SRC_DIR}/RendererManager.cpp
        ${RENDERER_LIB_INC_DIR}/WindowSurface.hpp
        ${RENDERER_LIB_INC_DIR}/VulkanUtility.hpp
        ${RENDERER_LIB_SRC_DIR}/VulkanUtility.cpp
    )
elseif(${PLATFORM_LINUX})
    set(RENDERER_LIB_SRC
        ${RENDERER_LIB_SRC_DIR}/DeviceVk.cpp
        ${RENDERER_LIB_SRC_DIR}/RendererVk.cpp
        ${RENDERER_LIB_INC_DIR}/DeviceVk.hpp
        ${RENDERER_LIB_INC_DIR}/RendererVk.hpp
        ${RENDERER_LIB_INC_DIR}/IRenderer.hpp
        ${RENDERER_LIB_INC_DIR}/RendererManager.hpp
        ${RENDERER_LIB_SRC_DIR}/RendererManager.cpp
        ${RENDERER_LIB_INC_DIR}/RendererManager.hpp
        ${RENDERER_LIB_INC_DIR}/VulkanUtility.hpp
        ${RENDERER_LIB_SRC_DIR}/VulkanUtility.cpp
        ${RENDERER_LIB_INC_DIR}/XlibInclude.hpp
    )
else()
    message(FATAL_ERROR "Unknown platform")
endif()

add_library(RENDERER_LIB STATIC ${RENDERER_LIB_SRC})

target_link_libraries(RENDERER_LIB PRIVATE COMMON_LIB
                                   PRIVATE ${Vulkan_LIBRARIES})

target_include_directories(RENDERER_LIB PRIVATE ${COMMON_LIB_INC_DIR}
                                        PUBLIC ${RENDERER_LIB_INC_DIR}
                                        PRIVATE ${X11_INCLUDE_DIRS}
                                        PRIVATE ${Vulkan_INCLUDE_DIRS}
                                        )

if(${PLATFORM_LINUX})
    target_link_libraries(RENDERER_LIB PRIVATE ${X11_LIBRARIES})
endif()
