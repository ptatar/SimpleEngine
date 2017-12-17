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
    class CommandPoolHandler;
    class SwapchainWrapper;
    class TimeUnits;

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

        class Swapchain
        {
            public:
                Swapchain();

                ~Swapchain();

                Status AcquireImage(TimeUnits& timeUnits);

                void PresentImage();

            private:
                VkSwapchainKHR GetSwapchain() { return m_swapchain; }

                VkSemaphore GetSemaphore() { return m_semaphore; }

                void AddImageIndex(Uint32 imageIndex) { m_currentImageIndex = imageIndex; }

            private:
                DeviceVk* m_device;
                VkSwapchainKHR m_swapchain;
                VkSemaphore m_semaphore;
                Uint32 m_currentImageIndex; // TODO maybe list of images if we acquire more then one image
                friend DeviceVk;
        };

        DeviceVk()
            : m_CreateDebugReportCallback(nullptr)
            , m_DestroyDebugReportCallback(nullptr)
            , m_debugCallback(0) {};

        ~DeviceVk() {};

        Bool Initialize();

        Bool CreateInstance();

        Bool CreateDevice();

        void Finalize();

#if defined(PLATFORM_WINDOWS)
        Result<SurfaceHandler> CreateSurface(IWindowSurface32* windowSurface);
#elif defined(PLATFORM_LINUX)
        Result<SurfaceHandler> CreateSurface(IWindowSurfaceX* windowSurface);
#endif

        void ClearScreenTest(VkSwapchainKHR swaochain);

        Result<SwapchainHandler> CreateSwapchain(SwapchainCreateInfo& createInfo);

        Result<SemaphoreHandler> CreateSemaphore();

        Result<CommandPoolHandler> CreateCommandPool();

        Result<std::vector<VkCommandBuffer>> AllocateCommandBuffers(VkSwapchainKHR swapchain,
                                                                    VkCommandPool commandPool);

        void DestroySemaphore(VkSemaphore semaphore) { vkDestroySemaphore(m_device, semaphore, nullptr); }

        void DestroySurface(VkSurfaceKHR surface) { vkDestroySurfaceKHR(m_instance, surface, nullptr); }

        void DestroySwapchain(VkSwapchainKHR swapchain) { vkDestroySwapchainKHR(m_device, swapchain, nullptr); }

        void DestroyCommandPool(VkCommandPool commandPool) { vkDestroyCommandPool(m_device, commandPool, nullptr); }

        std::vector<VkPresentModeKHR> GetSupporedPresentModes(VkSurfaceKHR surface) const;

        std::vector<VkSurfaceFormatKHR> GetSupportedSurfaceFormats(VkSurfaceKHR surface) const;

        Bool CheckAdapterSurfaceSupport(VkSurfaceKHR surface);

        Bool GetSurfaceCapabilities(VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR& capabilities) const;

    private:

        std::string AdapterPropertiesToString(const VkPhysicalDeviceProperties& adapterProperties) const;

        std::string AdapterTypeToString(const VkPhysicalDeviceType& adpaterType) const;

        std::string QueueFamilyToString(const VkQueueFamilyProperties& queueFamily) const;

        std::vector<const char*> GetRequiredInstanceExtension() const;

        std::vector<const char*> GetRequiredInstanceLayers() const;

        Bool CheckExtensionSupport(std::vector<const char*>& requiredExtensions,
                                   std::vector<VkExtensionProperties>& availableExtensions);

        Bool CheckInstanceExtensionsSupport(std::vector<const char*>& requiredExtensions);

        Bool CheckDeviceExtensionsSupport(VkPhysicalDevice& adapter,
                                          std::vector<const char*>& requiredExtensions);

        Bool CheckInstanceLayersSupport(std::vector<const char*>& requiredLayers) const;

        Bool SetupDebugCallback();

        void FinalizeDebugCallback();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
                                                            VkDebugReportObjectTypeEXT objType,
                                                            uint64_t obj,
                                                            size_t location,
                                                            int32_t code,
                                                            const char* layerPrefix,
                                                            const char* msg,
                                                            void* userData);

        std::vector<const char*> GetRequiredFunctionNames() const;

        Bool LoadFunctionPointers();

        Status AcquireSwapchainImage(Swapchain& swapchain, TimeUnits& timemout);

    private:
        VkInstance m_instance;
        VkDevice m_device;
        VkPhysicalDevice m_adapter;
        VkCommandBuffer m_commandBuffer;
        Uint32 m_queueFamilyIndex;

        PFN_vkCreateDebugReportCallbackEXT m_CreateDebugReportCallback;
        PFN_vkDestroyDebugReportCallbackEXT m_DestroyDebugReportCallback;
        VkDebugReportCallbackEXT m_debugCallback;
    }; // DeviceVK


    // I don't think it was even worth to write this thing
#define CREATE_HANDLER(NAME, TYPE, DESTRUCTOR) \
    class NAME \
    { \
    public: \
        NAME(): m_device(nullptr) {} \
        NAME(DeviceVk* device, TYPE type): m_device(device), m_type(type) {} \
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
        TYPE& Get() \
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
    CREATE_HANDLER(CommandPoolHandler, VkCommandPool, DestroyCommandPool);

#undef CREATE_HANDLER

} // namespace engine
