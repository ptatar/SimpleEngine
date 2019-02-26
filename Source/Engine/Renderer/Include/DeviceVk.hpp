#pragma once

#include <string>
#include <vector>

#include "WindowSurface.hpp"
#include "VulkanUtility.hpp"
#include "Handle.hpp"
#include "IImage.hpp"

namespace engine
{

    class SemaphoreG;
    class SurfaceG;
    class CommandPoolG;
    class ImageViewG;

    class SwapchainWrapper;
    class TimeUnits;
    class CommandDispatcher;
    class SwapchainVk;
    class CommandBufferVk;

    struct SwapchainCreateInfo
    {
        VkColorSpaceKHR colorSpace;
        VkFormat surfaceFormat;
        VkSurfaceTransformFlagBitsKHR transformation;
        Uint32 imageWidth;
        Uint32 imageHeight;
        Uint32 imageCount;
        VkImageUsageFlags usage;
        VkPresentModeKHR presentationMode;
        VkSurfaceKHR surface;
    };

    class DeviceVk
    {
        CLASS_NOT_COPYABLE(DeviceVk);
    public:

        DeviceVk()
            : m_CreateDebugReportCallback(nullptr)
            , m_DestroyDebugReportCallback(nullptr)
            , m_debugCallback(0) {};

        ~DeviceVk() {};

        Bool Initialize();

        Bool CreateInstance();

        Bool CreateDevice();

        ObjectRef<CommandDispatcher> CreateCommandDispatcher();

        void Finalize();

#if defined(PLATFORM_WINDOWS)
        Result<SurfaceH> CreateSurface(IWindowSurface32* windowSurface);
#elif defined(PLATFORM_LINUX)
        SurfaceG CreateSurface(IWindowSurfaceX* windowSurface);
#endif
        ObjectRef<SwapchainVk> CreateSwapchain(SwapchainCreateInfo& createInfo);

        SemaphoreG CreateSemaphore();

        FenceG CreateFence();

        CommandPoolG CreateCommandPool();

        std::vector<ObjectRef<CommandBufferVk>> AllocateCommandBuffers(Uint32 count,
                                                                     VkCommandPool commandPool);

        ShaderG CreateShader(const std::vector<Uint8>& data);

        void CreateRenderPass(const ObjectRef<SwapchainVk>& swapchain);

        void CreateFrameBuffer();

        void DestroySemaphore(VkSemaphore semaphore) { vkDestroySemaphore(m_device, semaphore, nullptr); }

        void DestroySurface(VkSurfaceKHR surface) { vkDestroySurfaceKHR(m_instance, surface, nullptr); }

        void DestroySwapchain(VkSwapchainKHR swapchain) { vkDestroySwapchainKHR(m_device, swapchain, nullptr); }

        void DestroyCommandPool(VkCommandPool commandPool) { vkDestroyCommandPool(m_device, commandPool, nullptr); }

        void DestroyImageView(VkImageView imageView) { vkDestroyImageView(m_device, imageView, nullptr); }

        void DestroyImage(VkImage image) { vkDestroyImage(m_device, image, nullptr); }

        void DestroyFence(VkFence fence) { vkDestroyFence(m_device, fence, nullptr); }

        void DestroyShader(VkShaderModule shader) { vkDestroyShaderModule(m_device, shader, nullptr); }

        std::vector<VkPresentModeKHR> GetSupporedPresentModes(VkSurfaceKHR surface) const;

        std::vector<VkSurfaceFormatKHR> GetSupportedSurfaceFormats(VkSurfaceKHR surface) const;

        Bool CheckAdapterSurfaceSupport(VkSurfaceKHR surface);

        Bool GetSurfaceCapabilities(VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR& capabilities) const;

        VkQueue GetCommandQueue() { return m_commandQueue; }

        VkQueue GetPresentQueue() { return m_commandQueue; }

        void SubmitQueue(ObjectRef<CommandBufferVk>& commandBuffer);

        Status WaitForFence(FenceG& fence, const TimeUnits& timeout);

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

        Status AcquireSwapchainImage(SwapchainVk& swapchain);

        ImageViewG CreateImageView(VkImage image);

        void BeginCommandBuffer(CommandBufferVk& commandBuffer);

        void EndCommandBuffer(CommandBufferVk& commandBuffer);

        VkQueue GetQueue(Uint32 queueFamilyIndex);

    private:
        VkInstance m_instance;

        VkDevice m_device;

        VkPhysicalDevice m_adapter;

        VkCommandBuffer m_commandBuffer;

        Uint32 m_queueFamilyIndex;

        VkQueue m_commandQueue;

        PFN_vkCreateDebugReportCallbackEXT m_CreateDebugReportCallback;

        PFN_vkDestroyDebugReportCallbackEXT m_DestroyDebugReportCallback;

        VkDebugReportCallbackEXT m_debugCallback;

        friend CommandBufferVk;

        friend SwapchainVk;

    }; // DeviceVK

    class CommandDispatcher
    {
        enum QueueType
        {
            Graphic,
            Transfer,
            Comput,
            SparseBinding,
        };
        class CommandQueueGroup
        {
            public:
                CommandQueueGroup() {}
                ~CommandQueueGroup() {}
            private:
                VkQueue m_queue;
                Uint32 m_count;
        };

        public:
            CommandDispatcher(DeviceVk* device): m_device(device) {}

            ~CommandDispatcher() {}


        private:
            DeviceVk* m_device;
            VkQueue m_queue;
    };


} // namespace engine
