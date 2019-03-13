#include "DeviceVk.hpp"

#include "Logger.hpp"
#include "Utility.hpp"
#include "CommandBufferVk.hpp"
#include "SwapchainVk.hpp"

#include <sstream>
#include <vector>
#include <cstring>

namespace engine
{

    Bool DeviceVk::Initialize()
    {
        if (!CreateInstance())
        {
            return false;
        }

        if (!LoadFunctionPointers())
        {
            return false;
        }

        if (!SetupDebugCallback())
        {
            return false;
        }

        if (!CreateDevice())
        {
            return false;
        }

        return true;
    }

    Bool DeviceVk::CreateInstance()
    {
        std::vector<const char*> requiredInstanceExtension = GetRequiredInstanceExtension();
        if (!CheckInstanceExtensionsSupport(requiredInstanceExtension))
        {
            return false;
        }

        std::vector<const char*> requiredInstanceLayers = GetRequiredInstanceLayers();
        if (!CheckInstanceLayersSupport(requiredInstanceLayers))
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
        instanceCreateInfo.enabledLayerCount = requiredInstanceLayers.size();
        instanceCreateInfo.ppEnabledLayerNames = requiredInstanceLayers.data();
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

        return true;
    }

    Bool DeviceVk::CreateDevice()
    {
        Uint32 adapterCount;
        VkResult result = vkEnumeratePhysicalDevices(m_instance, &adapterCount, NULL);
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
                queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                queueFamilyIndex = i;
            }
        }


        if (queueFamilyIndex >= queuePropertiesCount)
        {
            LOGE("Adapter queues don't fulfil minimal requirements");
            return false;
        }
        m_queueFamilyIndex = queueFamilyIndex;
        LOGI("Selected command queue family index %d", m_queueFamilyIndex);

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

        m_commandQueue = GetQueue(m_queueFamilyIndex);
        return true;
    }

    ObjectRef<CommandDispatcher> DeviceVk::CreateCommandDispatcher()
    {
        ASSERT(!m_adapter);
        ObjectRef<CommandDispatcher> commandDispatcher = MakeObjectRef<CommandDispatcher>(this);

        return commandDispatcher;
    }

    void DeviceVk::Finalize()
    {
        vkDestroyDevice(m_device, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }

#if defined(PLATFORM_WINDOWS)
    Result<SurfaceH> DeviceVk::CreateSurface(IWindowSurface32* windowSurface)
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
            return Status::Error;
        }
        return Result<SurfaceH>(Status::Success, SurfaceH(this, surface));
    }
#elif defined(PLATFORM_LINUX)
    SurfaceG DeviceVk::CreateSurface(IWindowSurfaceX* windowSurface)
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
            return SurfaceG();
        }

        return SurfaceG(this, surface);
    }
#endif

    ObjectRef<SwapchainVk> DeviceVk::CreateSwapchain(SwapchainCreateInfo& createInfo)
    {
        VkSwapchainCreateInfoKHR swapchainCreateInfo;
        swapchainCreateInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.pNext                 = nullptr;
        swapchainCreateInfo.flags                 = 0;
        swapchainCreateInfo.surface               = createInfo.surface;
        swapchainCreateInfo.minImageCount         = createInfo.imageCount;
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

        VkSwapchainKHR swapchainHandle;
        VkResult result = vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &swapchainHandle);
        if(result != VK_SUCCESS)
        {
            LOGE("Swapchain creation failed: %d", result);
            return MakeObjectRef<SwapchainVk>();
        }

        ObjectRef<SwapchainVk> swapchain = MakeObjectRef<SwapchainVk>(
                this,
                swapchainHandle,
                createInfo.imageWidth,
                createInfo.imageHeight,
                createInfo.imageCount);

        Uint32 imageCount;
        result = vkGetSwapchainImagesKHR(m_device, swapchainHandle, &imageCount, nullptr);
        if (result != VK_SUCCESS)
        {
            LOGE("vkGetSwapchainImagesKHR failure: %d", result);
            return MakeObjectRef<SwapchainVk>();
        }

        ImageFormat format = NativeFormat2ImageFormat(createInfo.surfaceFormat);
        ImageDesc imageDesc(createInfo.imageWidth, createInfo.imageHeight, format);

        std::vector<VkImage> images(imageCount);
        result = vkGetSwapchainImagesKHR(m_device, swapchainHandle, &imageCount, images.data());
        if (result != VK_SUCCESS)
        {
            LOGE("vkGetSwapchainImagesKHR failure: %d", result);
            return MakeObjectRef<SwapchainVk>();
        }

        auto& swapchainImages = swapchain->m_images;
        for (int i = 0; i < images.size(); i++)
        {
            SemaphoreG& semaphore = swapchain->m_semaphoreRing.GetNext();
            semaphore = CreateSemaphore();
            if (!semaphore)
            {
                return MakeObjectRef<SwapchainVk>();
            }

            VkImageViewCreateInfo info;
            info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            info.pNext = nullptr;
            info.flags = 0;
            info.image = images[i];
            info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            info.format = createInfo.surfaceFormat;
            info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            info.subresourceRange.baseMipLevel = 0;
            info.subresourceRange.levelCount = 1;
            info.subresourceRange.baseArrayLayer = 0;
            info.subresourceRange.layerCount = 1;

            VkImageView view;
            result = vkCreateImageView(m_device, &info, nullptr, &view);
            if (result != VK_SUCCESS)
            {
                LOGE("vkCreateImageView failure: %d", result);
                return MakeObjectRef<SwapchainVk>();
            }

            swapchainImages.Emplace(i, this, images[i], view, imageDesc);
        }

        return swapchain;
    }


    SemaphoreG DeviceVk::CreateSemaphore()
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
            return SemaphoreG();
        }
        return SemaphoreG(this, semaphore);
    }

    FenceG DeviceVk::CreateFence()
    {
        VkFenceCreateInfo info;
        info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        info.pNext = nullptr;;
        info.flags = 0;
        VkFence fence;
        VkResult result = vkCreateFence(m_device, &info, nullptr, &fence);
        if (result != VK_SUCCESS)
        {
            LOGE("Fence creation failure: %d", result);
            return FenceG();
        }
        return FenceG(this, fence);
    }


    CommandPoolG DeviceVk::CreateCommandPool()
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo;
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.pNext = nullptr;
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolCreateInfo.queueFamilyIndex = m_queueFamilyIndex;

        VkCommandPool commandPool;
        VkResult result = vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &commandPool);
        if (result != VK_SUCCESS)
        {
            LOGE("Command Pool creation failed: %d", result);
            return CommandPoolG(nullptr, VK_NULL_HANDLE);
        }
        return CommandPoolG(this, commandPool);
    }

    VkQueue DeviceVk::GetQueue(Uint32 queueFamilyIndex)
    {
        VkQueue queue;
        vkGetDeviceQueue(m_device, queueFamilyIndex, 0, &queue);
        return queue;
    }

    std::vector<ObjectRef<CommandBufferVk>> DeviceVk::AllocateCommandBuffers(Uint32 count,
                                                                           VkCommandPool commandPool)
    {
        std::vector<VkCommandBuffer> commandBuffers(count);
        VkCommandBufferAllocateInfo info;
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.pNext = nullptr;
        info.commandPool = commandPool;
        info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        info.commandBufferCount = count;

        VkResult result = vkAllocateCommandBuffers(m_device, &info, commandBuffers.data());
        if (result != VK_SUCCESS)
        {
            LOGE("Command Buffers allocation failed: %d", result);
            return std::vector<ObjectRef<CommandBufferVk>>();
        }

        std::vector<ObjectRef<CommandBufferVk>> out(count);
        for (Uint32 i = 0; i < count; i++)
        {
            SemaphoreG semaphore = CreateSemaphore();
            if (!semaphore)
            {
                return std::vector<ObjectRef<CommandBufferVk>>();
            }
            FenceG fence = CreateFence();
            if (!fence)
            {
                return std::vector<ObjectRef<CommandBufferVk>>();
            }
            out[i] = MakeObjectRef<CommandBufferVk>(this, commandBuffers[i], semaphore, fence);
        }

        return out;
    }


    ShaderG DeviceVk::CreateShader(const std::vector<Uint8>& data)
    {
        VkShaderModuleCreateInfo info =
        {
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, // VkStructureType              sType;
            nullptr,                                     // const void*                  pNext;
            0,                                           // VkShaderModuleCreateFlags    flags;
            data.size(),                                 // size_t                       codeSize;
            reinterpret_cast<const Uint32*>(data.data()) // const uint32_t*              pCode;
        };

        VkShaderModule shader;
        VkResult result = vkCreateShaderModule(m_device, &info, nullptr, &shader);
        if (result != VK_SUCCESS)
        {
            LOGE("Shader creation failed: %d", result);
            return ShaderG();
        }

        return ShaderG(this, shader);
    }


    RenderPassG DeviceVk::CreateRenderPass(const std::vector<ImageVk>& colorImages)
    {
        std::vector<VkAttachmentDescription> attachmentDesc(colorImages.size());
        std::vector<VkAttachmentReference> attachmentRef;
        for (int i = 0; i < colorImages.size(); ++i)
        {
            const ImageVk& image = colorImages[i];
            VkFormat format = ImageFormat2NativeFormat(image.GetImageDesc().format);
            attachmentDesc.push_back(
            {
                0,                                // VkAttachmentDescriptionFlags    flags;
                format,                           // VkFormat                        format;
                VK_SAMPLE_COUNT_1_BIT,            // VkSampleCountFlagBits           samples;
                VK_ATTACHMENT_LOAD_OP_CLEAR,      // VkAttachmentLoadOp              loadOp;
                VK_ATTACHMENT_STORE_OP_STORE,     // VkAttachmentStoreOp             storeOp;
                VK_ATTACHMENT_LOAD_OP_DONT_CARE,  // VkAttachmentLoadOp              stencilLoadOp;
                VK_ATTACHMENT_STORE_OP_DONT_CARE, // VkAttachmentStoreOp             stencilStoreOp;
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,  // VkImageLayout                   initialLayout;
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR   // VkImageLayout                   finalLayout;
            });

            attachmentRef.push_back(
            {
                0,                                       // uint32_t         attachment;
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL // VkImageLayout    layout;
            });
        }

        VkSubpassDescription spDesc =
        {
            0,                               // VkSubpassDescriptionFlags       flags;
            VK_PIPELINE_BIND_POINT_GRAPHICS, // VkPipelineBindPoint             pipelineBindPoint;
            0,                               // uint32_t                        inputAttachmentCount;
            nullptr,                         // const VkAttachmentReference*    pInputAttachments;
            1,                               // uint32_t                        colorAttachmentCount;
            attachmentRef.data(),            // const VkAttachmentReference*    pColorAttachments;
            nullptr,                         // const VkAttachmentReference*    pResolveAttachments;
            nullptr,                         // const VkAttachmentReference*    pDepthStencilAttachment;
            0,                               // uint32_t                        preserveAttachmentCount;
            nullptr                          // const uint32_t*                 pPreserveAttachments;

        };

        VkRenderPass renderPass;
        VkRenderPassCreateInfo info =
        {
            VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, //VkStructureType                   sType;
            nullptr,                                   //const void*                       pNext;
            0,                                         //VkRenderPassCreateFlags           flags;
            1,                                         //uint32_t                          attachmentCount;
            attachmentDesc.data(),                     //const VkAttachmentDescription*    pAttachments;
            1,                                         //uint32_t                          subpassCount;
            &spDesc,                                   //const VkSubpassDescription*       pSubpasses;
            0,                                         //uint32_t                          dependencyCount;
            nullptr                                    //const VkSubpassDependency*        pDependencies;
        };
        VkResult result = vkCreateRenderPass(m_device, &info, nullptr, &renderPass);
        if (result != VK_SUCCESS)
        {
            LOGE("CreateRenderPass failure: %d", result);
            return {};
        }
        return RenderPassG(this, renderPass);
    }


    void DeviceVk::CreateFramebuffer(RenderPassG& renderPass,
                                     Uint32 width,
                                     Uint32 height,
                                     const std::vector<ImageVk>& colorImages)
    {
        ASSERT(colorImages.size());
        ASSERT(renderPass != VK_NULL_HANDLE);

        // its bad but im lazy
        std::vector<VkImageView> views(colorImages.size());
        for (int i = 0; i < colorImages.size(); ++i)
        {
            views[i] = colorImages[i].GetView();

            const ImageDesc& desc = colorImages[i].GetImageDesc();
            ASSERT(width <= desc.width && height <= desc.height);
        }

        VkFramebufferCreateInfo info =
        {
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, // VkStructureType             sType;
            nullptr,                             // const void*                 pNext;
            0,                                   // VkFramebufferCreateFlags    flags;
            renderPass.Get(),                    // VkRenderPass                renderPass;
            static_cast<uint32_t>(views.size()), // uint32_t                    attachmentCount;
            views.data(),                        // const VkImageView*          pAttachments;
            width,                               // uint32_t                    width;
            height,                              // uint32_t                    height;
            1,                                   // uint32_t                    layers;
        };

        VkFramebuffer framebuffer;
        VkResult result = vkCreateFramebuffer(m_device, &info, nullptr, &framebuffer);
        if (result != VK_SUCCESS)
        {
            LOGE("Framebuffer creation failure: %d", result);
            return;
        }
        return;
    }


    std::vector<VkPresentModeKHR> DeviceVk::GetSupporedPresentModes(VkSurfaceKHR surface) const
    {
        Uint32 presentModeCount = 0;
        VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(m_adapter,
                                                                    surface,
                                                                    &presentModeCount,
                                                                    nullptr);

        if (result != VK_SUCCESS)
        {
            LOGE("vkGetPhysucalDeviceSurfacePresentModesKHR failed: %d", result);
            return std::vector<VkPresentModeKHR>();
        }

        std::vector<VkPresentModeKHR> supportedPresentModes(presentModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(m_adapter,
                                                           surface,
                                                           &presentModeCount,
                                                           supportedPresentModes.data());
        if (result != VK_SUCCESS)
        {
            LOGE("vkGetPhysucalDeviceSurfacePresentModesKHR failed: %d", result);
            return std::vector<VkPresentModeKHR>();
        }

        return supportedPresentModes;
    }

    std::vector<VkSurfaceFormatKHR> DeviceVk::GetSupportedSurfaceFormats(VkSurfaceKHR surface) const
    {
        Uint32 formatCount;
        VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_adapter, surface, &formatCount,  nullptr);
        if(result != VK_SUCCESS)
        {
            LOGE("Adapter supported sufrace formats count query failed: %d", result);
            return std::vector<VkSurfaceFormatKHR>(0);
        }

        std::vector<VkSurfaceFormatKHR> supportedSurfaceFormats(formatCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_adapter,
                                                      surface,
                                                      &formatCount,
                                                      supportedSurfaceFormats.data());

        if(result != VK_SUCCESS)
        {
            LOGE("Adapter support surface foramts query failed: %d", result);
            return std::vector<VkSurfaceFormatKHR>(0);
        }

        return supportedSurfaceFormats;
    }

    Bool DeviceVk::CheckAdapterSurfaceSupport(VkSurfaceKHR surface)
    {
        Uint32 supported;
        VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(m_adapter,
                                                               m_queueFamilyIndex,
                                                               surface,
                                                               &supported);
        if (result)
        {
            LOGE("Adapter surface and queue support failed: %d", result);
            return false;
        }
        else if (!supported)
        {
            LOGE("Given queue can't render on this surface");
            return false;
        }
        return true;
    }

    Bool DeviceVk::GetSurfaceCapabilities(VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR& capabilities) const
    {
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_adapter,
                                                                    surface,
                                                                    &capabilities);

        if (result != VK_SUCCESS)
        {
            LOGE("vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed: %d", result);
            return false;
        }

        return true;
    }

    void DeviceVk::SubmitQueue(ObjectRef<CommandBufferVk>& commandBuffer)
    {
        vkResetFences(m_device, 1, &commandBuffer->m_fence.Get());
        VkSubmitInfo info;
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.pNext = nullptr;
        info.waitSemaphoreCount = commandBuffer->m_dependency.size();
        info.pWaitSemaphores = reinterpret_cast<VkSemaphore*>(commandBuffer->m_dependency.data());
        info.pWaitDstStageMask = commandBuffer->m_dependencyStage.data();
        info.commandBufferCount = 1;
        info.pCommandBuffers =  &commandBuffer->m_commandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &commandBuffer->m_semaphore.Get();
        VkResult result = vkQueueSubmit(m_commandQueue, 1, &info, commandBuffer->m_fence.Get());
        ASSERT(result == VK_SUCCESS);
        commandBuffer->ChangeState(CommandBufferVk::State::Submitted);
    }

    Status DeviceVk::WaitForFence(FenceG& fence, const TimeUnits& timeout)
    {
        VkResult result = vkWaitForFences(m_device, 1, &fence.Get(), true, timeout.GetNanoseconds());
        if (result == VK_SUCCESS)
        {
            return Status::Success;
        }
        else if (result == VK_TIMEOUT)
        {
            LOGW("Fence %lld wait timeout", fence.Get());
            return Status::Timeout;
        }
        else
        {
            LOGE("Fence %lld wait error %d", fence.Get(), result);
            return Status::Error;
        }
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
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
        #if defined(PLATFORM_WINDOWS)
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        #elif defined(PLATFORM_LINUX)
            VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
        #endif
        };
        return instanceRequiredExtensions;
    }

    std::vector<const char*> DeviceVk::GetRequiredInstanceLayers() const
    {
        std::vector<const char*> instanceRequiredLayers =
        {
            "VK_LAYER_LUNARG_standard_validation"
        };
        return instanceRequiredLayers;
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

    Bool DeviceVk::CheckInstanceLayersSupport(std::vector<const char*>& requiredLayers) const
    {
        Uint32 layerCount;
        VkResult result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        if (result != VK_SUCCESS)
        {
            LOGE("Layers enumeration failure: %d", result);
            return false;
        }

        std::vector<VkLayerProperties> availableLayers(layerCount);
        result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        if (result != VK_SUCCESS)
        {
            LOGE("Layers enumeration failure: %d", result);
            return false;
        }
        Bool found = false;
        for (auto& required: requiredLayers)
        {
            found = false;
            for (auto& available: availableLayers)
            {
                if (std::strcmp(required, available.layerName) == 0)
                {
                    LOGI("Found required layer: %s", required);
                    found = true;
                }
            }
            if(!found)
            {
                LOGE("Required layer not found: %d", required);
                return false;
            }
        }
        return true;
    }

    Bool DeviceVk::SetupDebugCallback()
    {
        if (!m_CreateDebugReportCallback)
        {
            return false;
        }

        VkDebugReportCallbackCreateInfoEXT createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        createInfo.pfnCallback = DeviceVk::DebugCallback;
        VkResult result = m_CreateDebugReportCallback(m_instance, &createInfo, nullptr, &m_debugCallback);
        if (result != VK_SUCCESS)
        {
            LOGE("CreateDebugReportCallback failure: %", result);
            return false;
        }
        return true;
    }

    void DeviceVk::FinalizeDebugCallback()
    {

        if (m_DestroyDebugReportCallback && m_debugCallback)
        {
            m_DestroyDebugReportCallback(m_instance, m_debugCallback, nullptr);
            m_debugCallback = 0;
        }
    }

    Bool DeviceVk::LoadFunctionPointers()
    {
        m_CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)
            vkGetInstanceProcAddr(m_instance,"vkCreateDebugReportCallbackEXT");
        if (!m_CreateDebugReportCallback)
        {
            return false;
        }
        m_DestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)
            vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");
        if (!m_DestroyDebugReportCallback)
        {
            return false;
        }

        return true;
    }

    Status DeviceVk::AcquireSwapchainImage(SwapchainVk& swapchain)
    {
        Uint32 imageIndex;
        SemaphoreG& semaphore = swapchain.m_semaphoreRing.GetNext();

        VkResult result = vkAcquireNextImageKHR(m_device,
                                                swapchain.GetSwapchain(),
                                                INT64_MAX,
                                                semaphore.Get(),
                                                VK_NULL_HANDLE,
                                                &imageIndex);
        switch(result)
        {
            case VK_SUCCESS:
                swapchain.m_images[imageIndex].AddDependency(SemaphoreH(semaphore));
                swapchain.AddImage(imageIndex);
                return Status::Success;
            case VK_TIMEOUT:
                return Status::Timeout;
            case VK_SUBOPTIMAL_KHR:
                LOGW("Swapchain should be recreated.");
                return Status::OutDated;
            case VK_ERROR_OUT_OF_DATE_KHR:
                LOGW("Swapchain must be recreated.");
                return Status::OutDated;
            default:
                LOGE("AcquireNextImageKHR failed: %d", result);
                ASSERT(false);
                return Status::Error;
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL DeviceVk::DebugCallback(VkDebugReportFlagsEXT flags,
                                                           VkDebugReportObjectTypeEXT objType,
                                                           uint64_t obj,
                                                           size_t location,
                                                           int32_t code,
                                                           const char* layerPrefix,
                                                           const char* msg,
                                                           void* userData)
        {
            LOGE("Layer %s: %s", layerPrefix, msg);
            return false;
        }

} // namespace engine
