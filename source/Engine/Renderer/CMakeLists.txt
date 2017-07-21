set(RENDERER_LIB_SRC_DIR ${RENDERER_LIB_DIR}/src)

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
        ${RENDERER_LIB_SRC_DIR}/Renderer.cpp
        ${RENDERER_LIB_SRC_DIR}/DeviceVk.cpp
        ${RENDERER_LIB_SRC_DIR}/RendererVk.cpp
        ${RENDERER_LIB_INC_DIR}/DeviceVk.hpp
        ${RENDERER_LIB_INC_DIR}/RendererVk.hpp
        ${RENDERER_LIB_INC_DIR}/Renderer.hpp
    )
elseif(${PLATFORM_LINUX})
    set(RENDERER_LIB_SRC
        ${RENDERER_LIB_SRC_DIR}/DeviceVk.cpp
        ${RENDERER_LIB_SRC_DIR}/RendererVk.cpp
        ${RENDERER_LIB_INC_DIR}/DeviceVk.hpp
        ${RENDERER_LIB_INC_DIR}/RendererVk.hpp
        ${RENDERER_LIB_INC_DIR}/Renderer.cpp
    )
else()
    message(FATAL_ERROR "Unknown platform")
endif()

#fixme]
set(VULKAN_SDK $ENV{VULKAN_SDK})
link_directories(RENDERER_LIB PRIVATE ${VULKAN_SDK}/lib)

add_library(RENDERER_LIB STATIC ${RENDERER_LIB_SRC})
target_link_libraries(RENDERER_LIB PRIVATE COMMON_LIB)
target_include_directories(RENDERER_LIB PRIVATE ${COMMON_LIB_INC_DIR})
target_include_directories(RENDERER_LIB PUBLIC ${RENDERER_LIB_INC_DIR})

#fixme

target_include_directories(RENDERER_LIB PRIVATE ${VULKAN_SDK}/include)
