#pragma once

#include <string>
#include <vector>

#include "WindowSurface.hpp"
#include "VulkanUtility.hpp"


namespace engine
{
    struct SwapchainCreateInfo
    {
        VkColorSpaceKHR colorSpace;
        VkFormat surfaceFormat;
        Uint32 imagesCount;
        VkSurfaceTransformFlagBitsKHR transformation;
        Uint32 imageWidth;
        Uint32 imageHeight;
        VkImageUsageFlags usage;
        VkPresentModeKHR presentationMode;
        VkSurfaceKHR surface;
    };

    class DeviceVk
    {
    public:
        DeviceVk() {};
        ~DeviceVk() {};

        Bool Initialize();
        void Shutdown();
#if defined(PLATFORM_WINDOWS)
        Result<VkSurfaceKHR> CreateSurface(IWindowSurface32* windowSurface);
#elif defined(PLATFORM_LINUX)
        Result<VkSurfaceKHR> CreateSurface(IWindowSurfaceX* windowSurface);
#endif
        Bool CreateSwapchain(SwapchainCreateInfo& createInfo);
        Result<VkSemaphore> CreateSemaphore();

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
        VkPhysicalDevice m_adapter;
        VkCommandPool m_commandPool;
        VkCommandBuffer m_commandBuffer;
    };

} // namespace engine
