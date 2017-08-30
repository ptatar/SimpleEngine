#include "DeviceVk.hpp"

#include "Logger.hpp"

#include <sstream>
#include <vector>
#include <cstring>

namespace engine
{

    Bool DeviceVk::Initialize()
    {
        std::vector<const char*> requiredInstanceExtension = GetRequiredInstanceExtension();
        if (!CheckInstanceExtensionsSupport(requiredInstanceExtension))
        {
            return false;
        }

        VkApplicationInfo applicationInfo;
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext = nullptr;
        applicationInfo.pApplicationName = "VkSimpleApp";
        applicationInfo.applicationVersion = 0;
        applicationInfo.pEngineName = "SimpleEngine";
        applicationInfo.engineVersion = 0;
        applicationInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo;
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount = 0;
        instanceCreateInfo.ppEnabledLayerNames = nullptr;
        instanceCreateInfo.enabledExtensionCount = requiredInstanceExtension.size();
        instanceCreateInfo.ppEnabledExtensionNames = requiredInstanceExtension.data();

        VkResult result = vkCreateInstance(&instanceCreateInfo, NULL, &m_instance);
        if (result == VK_ERROR_INCOMPATIBLE_DRIVER)
        {
            LOGE("Vk instance creation failure. Incopatible driver: %d", result);
            return false;
        }
        else if (result != VK_SUCCESS)
        {
            LOGE("Vk instance creation failure: %d", result);
            return false;
        }

        Uint32 adapterCount;
        result = vkEnumeratePhysicalDevices(m_instance, &adapterCount, NULL);
        if (result != VK_SUCCESS)
        {
            LOGE("Vk physical devices enumeration failure: %d", result);
            return false;
        }

        std::vector<VkPhysicalDevice> adapters(adapterCount);
        result = vkEnumeratePhysicalDevices(m_instance, &adapterCount, adapters.data());
        if (result != VK_SUCCESS)
        {
            LOGE("Vk adapters enumeration failure: %d", result);
            return false;
        }

        LOGI("Enumerated adapters:");
        Uint32 selectedAdapterIndex = adapterCount;
        std::vector<const char*> requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        for (Uint32 i = 0; i < adapters.size(); ++i)
        {
            VkPhysicalDeviceProperties adapterProperties;
            vkGetPhysicalDeviceProperties(adapters[i], &adapterProperties);
            LOGI("\n%s", AdapterPropertiesToString(adapterProperties).c_str());
            if (adapterProperties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
                adapterProperties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            {
                if (!CheckDeviceExtensionsSupport(adapters[i],requiredDeviceExtensions))
                {
                    continue;
                }

                LOGI("Adapter selected: \n%s", AdapterPropertiesToString(adapterProperties).c_str());
                selectedAdapterIndex = i;
                m_adapter = adapters[selectedAdapterIndex];
                break;
            }
        }
        if (selectedAdapterIndex >= adapterCount)
        {
            LOGE("Adapter selection failure");
            return false;
        }

        Uint32 queuePropertiesCount;
        vkGetPhysicalDeviceQueueFamilyProperties(m_adapter, &queuePropertiesCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queuePropertiesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_adapter, &queuePropertiesCount, queueFamilyProperties.data());

        LOGI("Listing queue families:");
        Uint32 queueFamilyIndex = queuePropertiesCount;
        for (Uint32 i = 0; i < queueFamilyProperties.size(); ++i)
        {
            LOGI("Queue Family %d\n%s", i, QueueFamilyToString(queueFamilyProperties[i]).c_str());
            if (queueFamilyProperties[i].queueCount > 0 &&
                queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT &&
                queueFamilyIndex == queuePropertiesCount)
            {
                queueFamilyIndex = i;
            }

        }

        if (queueFamilyIndex >= queuePropertiesCount)
        {
            LOGE("Adapter queues don't fulfil minimal requirements");
            return false;
        }
        LOGI("Selected queue family index %d", queueFamilyIndex);

        Float queuePriorities = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pNext = nullptr;
        queueCreateInfo.flags = 0;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriorities;

        VkDeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;
        deviceCreateInfo.enabledExtensionCount = requiredDeviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();
        deviceCreateInfo.pEnabledFeatures = nullptr;

        result = vkCreateDevice(m_adapter, &deviceCreateInfo, NULL, &m_device);
        if (result != VK_SUCCESS)
        {
            LOGE("Device creation failure: %d", result);
            return false;
        }

        VkCommandPoolCreateInfo commandPoolCreateInfo;
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.pNext = nullptr;
        commandPoolCreateInfo.flags = 0;
        commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

        result = vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_commandPool);
        if (result != VK_SUCCESS)
        {
            LOGE("Command pool creation failure: %d", result);
            return false;
        }

        VkCommandBufferAllocateInfo commandBufferAllocateInfo;
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.pNext = nullptr;
        commandBufferAllocateInfo.commandPool = m_commandPool;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount = 1;

        result = vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, &m_commandBuffer);
        if (result != VK_SUCCESS)
        {
            LOGE("Command buffer allocation failure: %d", result);
            return false;
        }

        return true;
    }

    void DeviceVk::Shutdown()
    {
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);
        vkDestroyDevice(m_device, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }

#if defined(PLATFORM_WINDOWS)
    Result<SurfaceHandler> DeviceVk::CreateSurface(IWindowSurface32* windowSurface)
    {
        VkSurfaceKHR surface;
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.pNext = nullptr;
        surfaceCreateInfo.flags = 0;
        surfaceCreateInfo.hinstance = windowSurface->GetHInstance();
        surfaceCreateInfo.hwnd = windowSurface->GetHWindow();
        VkResult result = vkCreateWin32SurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &surface);
        if (result != VK_SUCCESS)
        {
            LOGE("Surface creation failure: %d", result);
            return Status::Failure;
        }
        return Result<SurfaceHandler>(Status::Success, SurfaceHandler(this, surface));
    }
#elif defined(PLATFORM_LINUX)
    Result<SurfaceHandler> DeviceVk::CreateSurface(IWindowSurfaceX* windowSurface)
    {
        VkSurfaceKHR surface;
        VkXlibSurfaceCreateInfoKHR surfaceCreateInfo;
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.pNext = nullptr;
        surfaceCreateInfo.flags = 0;
        surfaceCreateInfo.window = windowSurface->GetWindow();
        surfaceCreateInfo.dpy = windowSurface->GetDisplay();
        VkResult result = vkCreateXlibSurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &surface);
        if (result != VK_SUCCESS)
        {
            LOGE("Surface creation failure: %d", result);
            return Status::Failure;
        }

        return Result<SurfaceHandler>(Status::Success, SurfaceHandler(this, surface));
    }
#endif

    Result<SwapchainHandler> DeviceVk::CreateSwapchain(SwapchainCreateInfo& createInfo)
    {

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_adapter,
                                                                    createInfo.surface,
                                                                    &surfaceCapabilities);
        if(createInfo.imageWidth > surfaceCapabilities.maxImageExtent.width ||
           createInfo.imageHeight > surfaceCapabilities.maxImageExtent.height)
        {
            LOGE("Invalid surface extents");
            return Status::Failure;
        }

        if(createInfo.imagesCount > surfaceCapabilities.maxImageCount ||
           createInfo.imagesCount < surfaceCapabilities.minImageCount)
        {
            LOGE("Invalid image count");
            return Status::Failure;
        }
        if(result != VK_SUCCESS)
        {
            LOGE("Adapter surface capabilities: %d", result);
            return Status::Failure;
        }

        Uint32 formatCount;
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_adapter, createInfo.surface, &formatCount,  nullptr);
        if(result != VK_SUCCESS)
        {
            LOGE("Adapter supported sufrace formats count query failed: %d", result);
            return Status::Failure;
        }

        std::vector<VkSurfaceFormatKHR> supportedSurfaceFormats(formatCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_adapter,
                                                      createInfo.surface,
                                                      &formatCount,
                                                      supportedSurfaceFormats.data());

        if(result != VK_SUCCESS)
        {
            LOGE("Adapter support surface foramts query failed: %d", result);
            return Status::Failure;
        }

        Bool formatFound = false;
        for(auto& surfaceFormat: supportedSurfaceFormats)
        {
            if(surfaceFormat.colorSpace == createInfo.colorSpace)
            {
                if(surfaceFormat.format == createInfo.surfaceFormat)
                {
                    formatFound = true;
                }
            }
        }

        if(!formatFound)
        {
            LOGE("Not supported format and color space combination");
            return Status::Failure;
        }

        Uint32 presentModeCount;
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(m_adapter,
                                                           createInfo.surface,
                                                           &presentModeCount,
                                                           nullptr);
        if(result != VK_SUCCESS)
        {
            LOGE("Presentation modes count query failed: %d", result);
            return Status::Failure;
        }

        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(m_adapter,
                                                           createInfo.surface,
                                                           &presentModeCount,
                                                           presentModes.data());
        if(result != VK_SUCCESS)
        {
            LOGE("Presentation modes query failed: %d", result);
            return Status::Failure;
        }

        Uint32 targetPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        for(auto& presentMode: presentModes)
        {
            if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                targetPresentMode = presentMode;
            }
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo;
        swapchainCreateInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.pNext                 = nullptr;
        swapchainCreateInfo.flags                 = 0;
        swapchainCreateInfo.surface               = createInfo.surface;
        swapchainCreateInfo.minImageCount         = 2;
        swapchainCreateInfo.imageFormat           = createInfo.surfaceFormat;
        swapchainCreateInfo.imageColorSpace       = createInfo.colorSpace;
        swapchainCreateInfo.imageExtent           = VkExtent2D{createInfo.imageWidth, createInfo.imageHeight};
        swapchainCreateInfo.imageArrayLayers      = 1;
        swapchainCreateInfo.imageUsage            = createInfo.usage;
        swapchainCreateInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices   = 0;
        swapchainCreateInfo.preTransform          = createInfo.transformation;
        swapchainCreateInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode           = createInfo.presentationMode;
        swapchainCreateInfo.clipped               = VK_TRUE;
        swapchainCreateInfo.oldSwapchain          = 0;

        VkSwapchainKHR swapchain;
        result = vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &swapchain);
        if(result != VK_SUCCESS)
        {
            LOGE("Swapchain creation failed: %d", result);
            return Status::Failure;
        }
        return Result<SwapchainHandler>(Status::Success, SwapchainHandler(this, swapchain));
    }

    Result<SemaphoreHandler> DeviceVk::CreateSemaphore()
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo;
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext = nullptr;
        semaphoreCreateInfo.flags = 0;

        VkSemaphore semaphore;
        VkResult result = vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &semaphore);
        if (result != VK_SUCCESS)
        {
            LOGE("Semaphore creation failure: %d", result);
            return Status::Failure;
        }
        return Result<SemaphoreHandler>(Status::Success, SemaphoreHandler(this, semaphore) );
    }

    std::string DeviceVk::AdapterPropertiesToString(const VkPhysicalDeviceProperties& adapterProperties) const
    {
        std::stringstream buf;
        buf << "Name:        " << adapterProperties.deviceName << "\n"
            << "Vendor:      " << adapterProperties.vendorID << "\n"
            << "Api version: " << adapterProperties.apiVersion << "\n"
            << "Device ID:   " << adapterProperties.deviceID << "\n"
            << "Device Type: " << AdapterTypeToString(adapterProperties.deviceType);

        return buf.str();
    }

    std::string DeviceVk::AdapterTypeToString(const VkPhysicalDeviceType& adpaterType) const
    {
        switch (adpaterType)
        {
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
                return "CPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                return "DISCRETE_GPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                return "INTEGRETED_GPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                return "VIRTUAL_GPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER:
                return "OTHER";
            default:
                return "UNKNOWN";
        }
    }

    std::string DeviceVk::QueueFamilyToString(const VkQueueFamilyProperties& queueFamily) const
    {
        std::stringstream buf;
        buf << "Queues count:                          " << queueFamily.queueCount << "\n"
            << "Queue flag:                            "
            << (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT ? "GRAPHICS_BIT " : "")
            << (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT ? "COMPUTE_BIT " : "")
            << (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT ? "TRANSFER_BIT " : "")
            << (queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT ? "SPARSE_BINDING_BIT" : "") << "\n"
            << "Queue timestamp valid bits:            " << queueFamily.timestampValidBits << "\n"
            << "Queue min transfer granularity (w/h/d):" << queueFamily.minImageTransferGranularity.width << " "
            << queueFamily.minImageTransferGranularity.height << " "
            << queueFamily.minImageTransferGranularity.depth << " ";
        return buf.str();
    }

    std::vector<const char*> DeviceVk::GetRequiredInstanceExtension() const
    {
        std::vector<const char*> instanceRequiredExtensions =
        {
            VK_KHR_SURFACE_EXTENSION_NAME,
        #if defined(PLATFORM_WINDOWS)
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        #elif defined(PLATFORM_LINUX)
            VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
        #endif
        };
        return instanceRequiredExtensions;
    }

    Bool DeviceVk::CheckExtensionSupport(std::vector<const char*>& requiredExtensions, std::vector<VkExtensionProperties>& availableExtensions)
    {
        std::vector<Bool> extensionCheckResult(requiredExtensions.size(), false);
        Bool extensionSupportError = false;
        for (Uint32 i = 0; i < requiredExtensions.size(); ++i)
        {
            Bool supported = false;
            const char* required = requiredExtensions[i];
            for (auto& available : availableExtensions)
            {
                if (std::strcmp(required, available.extensionName) == 0)
                {
                    supported = true;
                    break;
                }
            }
            if (supported == false)
            {
                LOGE("Required extension not supported: %s", requiredExtensions[i]);
                extensionSupportError = true;
            }
        }
        return !extensionSupportError;
    }

    Bool DeviceVk::CheckInstanceExtensionsSupport(std::vector<const char*>& requiredExtensions)
    {
        Uint32 extensionCount;
        VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        if (result != VK_SUCCESS)
        {
            LOGE("Extension enumeration failure: %d", result);
            return false;
        }

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
        if (result != VK_SUCCESS)
        {
            LOGE("Extension enumeration failure: %d", result);
            return false;
         }
        return CheckExtensionSupport(requiredExtensions, availableExtensions);
    }

    Bool DeviceVk::CheckDeviceExtensionsSupport(VkPhysicalDevice& adapter, std::vector<const char*>& requiredExtensions)
    {
        Uint32 extensionCount;
        VkResult result = vkEnumerateDeviceExtensionProperties(adapter, nullptr, &extensionCount, nullptr);
        if (result != VK_SUCCESS)
        {
            LOGE("Device extension enumeration failure: %d", result);
            return false;
        }
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        result = vkEnumerateDeviceExtensionProperties(adapter, nullptr, &extensionCount, availableExtensions.data());
        if (result != VK_SUCCESS)
        {
            LOGE("Device extension enumeration failure: %d", result);
            return false;
        }
        return CheckExtensionSupport(requiredExtensions, availableExtensions);
    }

} // namespace engine
