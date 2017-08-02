#pragma once

#include "Types.hpp"
#if defined(PLATFORM_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR
#elif defined(PLATFORM_LINUX)
    #define VK_USE_PLATFORM_XLIB_KHR
#endif
#include "vulkan/vulkan.h"

#include <string>
#include <vector>


namespace engine
{

    class DeviceVk
    {
    public:
        DeviceVk() {};
        ~DeviceVk() {};

        Bool Initialize();
        void Shutdown();
#if defined(PLATFORM_WINDOWS)
		Bool CreateSurface32();
#elif defined(PLATFORM_LINUX)
        Bool CreateSurfaceX();
#endif
    private:
        std::string AdapterPropertiesToString(const VkPhysicalDeviceProperties& adapterProperties) const;
        std::string AdapterTypeToString(const VkPhysicalDeviceType& adpaterType) const;
		std::string QueueFamilyToString(const VkQueueFamilyProperties& queueFamily) const;
        std::vector<const char*> GetRequiredInstanceExtension() const;

    private:
        VkInstance m_instance;
		VkDevice m_device;
		VkCommandPool m_commandPool;
		VkCommandBuffer m_commandBuffer;
    };

} // namespace engine
