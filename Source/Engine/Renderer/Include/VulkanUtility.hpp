#pragma once

#if defined(PLATFORM_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <vulkan/vulkan.h>
#elif defined(PLATFORM_LINUX)
    #define VK_USE_PLATFORM_XLIB_KHR
    #include <vulkan/vulkan.h>
    #include <XlibInclude.hpp>
#endif

#include "Types.hpp" // DO NOT MOVE IT BEFORE X11!!!


namespace engine
{
    enum class ImageFormat;

    VkFormat ImageFormat2NativeFormat(ImageFormat format);


} // namespace engine
