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
    enum class Status
    {
        Success,
        Failure,
    };

    template <typename T>
    struct Result
    {
        Result(Status _status): status(_status) {}
        Result(Status _status, T& _value): status(_status), value(_value) {}
        Result(Status _status, T&& _value): status(_status), value(std::move(value)) {}
        Status status;
        T value;
    };

} // namespace engine
