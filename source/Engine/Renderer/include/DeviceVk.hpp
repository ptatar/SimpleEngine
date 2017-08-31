#pragma once

#include <string>
#include <vector>

#include "WindowSurface.hpp"
#include "VulkanUtility.hpp"


namespace engine
{
   
    class SemaphoreHandler;
    class SurfaceHandler;
    class SwapchainHandler;

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
        Result<SurfaceHandler> CreateSurface(IWindowSurface32* windowSurface);
#elif defined(PLATFORM_LINUX)
        Result<SurfaceHandler> CreateSurface(IWindowSurfaceX* windowSurface);
#endif
        Result<SwapchainHandler> CreateSwapchain(SwapchainCreateInfo& createInfo);
        Result<SemaphoreHandler> CreateSemaphore();
        void DestroySemaphore(VkSemaphore semaphore) { vkDestroySemaphore(m_device, semaphore, nullptr); }
        void DestroySurface(VkSurfaceKHR surface) { vkDestroySurfaceKHR(m_instance, surface, nullptr); }
        void DestroySwapchain(VkSwapchainKHR swapchain) { vkDestroySwapchainKHR(m_device, swapchain, nullptr); }

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
    }; // DeviceVK

#define CREATE_HANDLER(NAME, TYPE, DESTRUCTOR) \
    class NAME \
    { \
    public: \
        NAME(): m_device(nullptr) {} \
        NAME(DeviceVk* device, TYPE type):m_device(device), m_type(type) {} \
        NAME(NAME&& rs) \
        { \
            m_type = rs.m_type; \
            m_device = rs.m_device; \
            rs.m_device = nullptr; \
        } \
        NAME(const NAME&) = delete; \
        ~NAME() \
        { \
            if(m_device) \
            { \
                m_device->DESTRUCTOR(m_type); \
            } \
        } \
        NAME operator=(const NAME&) = delete; \
        NAME operator=(NAME&& rs) \
        { \
            if(rs.m_device) \
            { \
                DeviceVk* ptr = rs.m_device; \
                rs.m_device = nullptr; \
                return NAME(ptr, rs.m_type); \
            } \
            else \
            { \
                return NAME(); \
            } \
        } \
        TYPE Get() const \
        { \
            return m_type; \
        } \
    private: \
        TYPE m_type; \
        DeviceVk* m_device; \
    }

    CREATE_HANDLER(SemaphoreHandler, VkSemaphore, DestroySemaphore);
    CREATE_HANDLER(SurfaceHandler, VkSurfaceKHR, DestroySurface);
    CREATE_HANDLER(SwapchainHandler, VkSwapchainKHR, DestroySwapchain);

#undef CREATE_HANDLER

} // namespace engine
