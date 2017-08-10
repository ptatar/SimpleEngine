#pragma once

#include "Types.hpp"


#if defined(PLATFORM_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR
#elif defined(PLATFORM_LINUX)
    #define VK_USE_PLATFORM_XLIB_KHR
#endif
#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#include "WindowSurface.hpp"

#undef Status
#undef Success

namespace engine
{

    class DeviceVk
    {
    public:
        enum class Status
        {
            Success,
            Failure,
        };

        template <typename T>
        struct Result
        {
            Status status;
            T value;
        };

        DeviceVk() {};
        ~DeviceVk() {};

        Bool Initialize();
        void Shutdown();
#if defined(PLATFORM_WINDOWS)
        Result<VkSurfaceKHR> CreateSurface(IWindowSurface32* windowSurface);
#elif defined(PLATFORM_LINUX)
        Result<VkSurfaceKHR> CreateSurface(IWindowSurfaceX* windowSurface);
#endif
    private:
        std::string AdapterPropertiesToString(const VkPhysicalDeviceProperties& adapterProperties) const;
        std::string AdapterTypeToString(const VkPhysicalDeviceType& adpaterType) const;
        std::string QueueFamilyToString(const VkQueueFamilyProperties& queueFamily) const;
        std::vector<const char*> GetRequiredInstanceExtension() const;
        Bool CheckExtensionSupport(std::vector<const char*>& requiredExtensions, std::vector<VkExtensionProperties>& availableExtensions);
        Bool CheckInstanceExtensionsSupport(std::vector<const char*>& requiredExtensions);
        Bool CheckDeviceExtensionsSupport(VkPhysicalDevice& adapter, std::vector<const char*>& requiredExtensions);

    private:
        VkInstance m_instance;
        VkDevice m_device;
        VkCommandPool m_commandPool;
        VkCommandBuffer m_commandBuffer;
    };

} // namespace engine
