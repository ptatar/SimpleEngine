#pragma once

#include "Types.hpp"
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
    private:
        std::string AdapterPropertiesToString(const VkPhysicalDeviceProperties& adapterProperties) const;
        std::string AdapterTypeToString(const VkPhysicalDeviceType& adpaterType) const;
		std::string QueueFamilyToString(const VkQueueFamilyProperties& queueFamily) const;
    private:
        VkInstance m_instance;
    };

} // namespace engine
