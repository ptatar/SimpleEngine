#pragma once

#include "Types.hpp"
#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"

#include <string>

namespace engine
{

    class DeviceVk
    {
        struct Version
        {
            Uint32 major;
            Uint32 minor;
            Uint32 patch;
        };
    public:
        DeviceVk() {};
        ~DeviceVk() {};

        Bool Initialize();
        void Shutdown();

		Bool CreateSurface32();
    private:
        std::string AdapterPropertiesToString(const VkPhysicalDeviceProperties& adapterProperties) const;
        std::string AdapterTypeToString(const VkPhysicalDeviceType& adpaterType) const;
		std::string QueueFamilyToString(const VkQueueFamilyProperties& queueFamily) const;
    private:
        VkInstance m_instance;
		VkDevice m_device;
		VkCommandPool m_commandPool;
		VkCommandBuffer m_commandBuffer;
    };

} // namespace engine
