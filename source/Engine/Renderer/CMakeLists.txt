set(RENDERER_LIB_SRC_DIR ${RENDERER_LIB_DIR}/src)

include(FindVulkan)
if(NOT ${Vulkan_FOUND})
    message(FATAL_ERROR "Vulkan not found")
endif()

if(${PLATFORM_WINDOWS})
    set(RENDERER_LIB_SRC
#        ${RENDERER_LIB_SRC_DIR}/DeviceDX12.cpp
#        ${RENDERER_LIB_SRC_DIR}/HardwareManagerDX12.cpp
#        ${RENDERER_LIB_SRC_DIR}/RendererDX12.cpp
#        ${RENDERER_LIB_SRC_DIR}/SwapChainDX12.cpp
#        ${RENDERER_LIB_INC_DIR}/DeviceDX12.hpp
#        ${RENDERER_LIB_INC_DIR}/HardwareManagerDX12.hpp
#        ${RENDERER_LIB_INC_DIR}/RendererDX12.hpp
#        ${RENDERER_LIB_INC_DIR}/SwapChainDX12.hpp
        ${RENDERER_LIB_SRC_DIR}/DeviceVk.cpp
        ${RENDERER_LIB_SRC_DIR}/RendererVk.cpp
        ${RENDERER_LIB_INC_DIR}/DeviceVk.hpp
        ${RENDERER_LIB_INC_DIR}/RendererVk.hpp
        ${RENDERER_LIB_INC_DIR}/IRenderer.hpp
        ${RENDERER_LIB_INC_DIR}/RendererManager.hpp
        ${RENDERER_LIB_SRC_DIR}/RendererManager.cpp
        ${RENDERER_LIB_INC_DIR}/WindowSurface.hpp
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
    )
else()
    message(FATAL_ERROR "Unknown platform")
endif()
message(${Vulkan_INCLUDE_DIRS})

add_library(RENDERER_LIB STATIC ${RENDERER_LIB_SRC})

target_link_libraries(RENDERER_LIB PRIVATE COMMON_LIB
                                   PRIVATE ${Vulkan_LIBRARIES})

target_include_directories(RENDERER_LIB PRIVATE ${COMMON_LIB_INC_DIR}
                                        PUBLIC ${RENDERER_LIB_INC_DIR}
                                        PRIVATE ${X11_INCLUDE_DIR}
                                        PRIVATE ${Vulkan_INCLUDE_DIRS}
                                        )

if(${PLATFORM_LINUX})
    target_link_libraries(RENDERER_LIB PRIVATE ${X11_LIBRARIES})
endif()
