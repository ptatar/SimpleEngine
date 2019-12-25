set(RENDERER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(RENDERER_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set(LIBRARY_INC_DIR ${RENDERER_INC_DIR} PARENT_SCOPE)

if(${PLATFORM_WINDOWS})
    set(RENDERER_SRC
        ${RENDERER_SRC_DIR}/DeviceVk.cpp
        ${RENDERER_SRC_DIR}/RendererVk.cpp
        ${RENDERER_INC_DIR}/DeviceVk.hpp
        ${RENDERER_INC_DIR}/RendererVk.hpp
        ${RENDERER_INC_DIR}/IRenderer.hpp
        ${RENDERER_INC_DIR}/RendererManager.hpp
        ${RENDERER_SRC_DIR}/RendererManager.cpp
        ${RENDERER_INC_DIR}/WindowSurface.hpp
        ${RENDERER_INC_DIR}/VulkanUtility.hpp
        ${RENDERER_SRC_DIR}/VulkanUtility.cpp
    )
elseif(${PLATFORM_LINUX})
    set(RENDERER_SRC
        ${RENDERER_SRC_DIR}/DeviceVk.cpp
        ${RENDERER_INC_DIR}/DeviceVk.hpp
        ${RENDERER_INC_DIR}/IRenderer.hpp
        ${RENDERER_INC_DIR}/RendererVk.hpp
        ${RENDERER_SRC_DIR}/RendererVk.cpp
        ${RENDERER_INC_DIR}/RendererManager.hpp
        ${RENDERER_SRC_DIR}/RendererManager.cpp
        ${RENDERER_INC_DIR}/VulkanUtility.hpp
        ${RENDERER_SRC_DIR}/VulkanUtility.cpp
        ${RENDERER_INC_DIR}/XlibInclude.hpp
        ${RENDERER_INC_DIR}/ICommandBuffer.hpp
        ${RENDERER_INC_DIR}/CommandBufferVk.hpp
        ${RENDERER_SRC_DIR}/CommandBufferVk.cpp
        ${RENDERER_INC_DIR}/ISwapchain.hpp
        ${RENDERER_INC_DIR}/SwapchainVk.hpp
        ${RENDERER_SRC_DIR}/SwapchainVk.cpp
        ${RENDERER_INC_DIR}/FramebufferVk.hpp
        ${RENDERER_SRC_DIR}/FramebufferVk.cpp
        ${RENDERER_INC_DIR}/IImage.hpp
        ${RENDERER_INC_DIR}/ImageVk.hpp
        ${RENDERER_SRC_DIR}/ImageVk.cpp
        ${RENDERER_INC_DIR}/StateCacheVk.hpp
        ${RENDERER_SRC_DIR}/StateCacheVk.cpp
        ${RENDERER_INC_DIR}/ShaderManager.hpp
        ${RENDERER_SRC_DIR}/ShaderManager.cpp
        ${RENDERER_INC_DIR}/ShaderVk.hpp
        ${RENDERER_SRC_DIR}/ShaderVk.cpp
        ${RENDERER_INC_DIR}/GeometryVk.hpp
        ${RENDERER_SRC_DIR}/GeometryVk.cpp
        ${RENDERER_INC_DIR}/Handle.hpp
        ${RENDERER_SRC_DIR}/Handle.cpp
    )
else()
    message(FATAL_ERROR "Unknown platform")
endif()

add_library(RENDERER STATIC ${RENDERER_SRC})

target_link_libraries(RENDERER PRIVATE COMMON
                               PRIVATE THREAD
                               PRIVATE ${Vulkan_LIBRARIES})


target_include_directories(RENDERER PRIVATE ${COMMON_INC_DIR}
                                    PRIVATE ${THREAD_INC_DIR}
                                    PRIVATE ${FILE_MANAGER_INC_DIR}
                                    PUBLIC ${RENDERER_INC_DIR}
                                    PRIVATE ${X11_INCLUDE_DIRS}
                                    PUBLIC ${Vulkan_INCLUDE_DIRS}
                                    )

if(${PLATFORM_LINUX})
    target_link_libraries(RENDERER PRIVATE ${X11_LIBRARIES})
endif()
