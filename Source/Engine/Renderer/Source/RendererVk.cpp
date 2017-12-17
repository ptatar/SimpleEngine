#include "RendererVk.hpp"

#include "Utility.hpp"

namespace engine
{

    Bool RendererVk::Initialize()
    {
        return m_device.Initialize(); // remove device from renderer
    }

    void RendererVk::Finalize()
    {
        m_device.Finalize();
    }

    void RendererVk::Update()
    {
    }

#if defined(PLATFORM_WINDOWS)
    Bool RendererVk::CreateSurface(IWindowSurface32* windowSurface)
    {
        auto surfaceResult = m_device.CreateSurface(windowSurface);
        ASSERT_RETURN(surfaceResult.status == Status::Success);

        auto semaphoreImageResult = m_device.CreateSemaphore();
        ASSERT_RETURN(semaphoreImageResult.status != Status::Success);

        auto semaphoreRenderingResult = m_device.CreateSemaphore();
        ASSERT_RETURN(semaphoreRenderingResult.status != Status::Success)

        VkPresentModeKHR targetPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        CheckPresentModeSupported(m_device.GetSuppo
        SwapchainCreateInfo swapchainCreateInfo;
        swapchainCreateInfo.colorSpace       = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        swapchainCreateInfo.surfaceFormat    = VK_FORMAT_R8G8B8A8_UNORM;
        swapchainCreateInfo.imagesCount      = 1;
        swapchainCreateInfo.transformation   = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapchainCreateInfo.imageWidth       = windowSurface->GetSurfaceExtent().width;
        swapchainCreateInfo.imageHeight      = windowSurface->GetSurfaceExtent().height;
        swapchainCreateInfo.usage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainCreateInfo.presentationMode = VK_PRESENT_MODE_MAILBOX_KHR;
        swapchainCreateInfo.surface          = surfaceResult.value.Get();


        auto swapchainResult = m_device.CreateSwapchain(swapchainCreateInfo);
        ASSERT_RETURN(swapchainResult.status == Status::Success);

        auto commandPoolResult = m_device.CreateCommandPool();
        ASSERT_RETURN(commandPoolResult.status == Status::Success);

        auto commandBuffersResult = m_device.AllocateCommandBuffers(swapchainResult.value.Get(),
                                                                    commandPoolResult.value.Get());
        ASSERT_RETURN(commandBuffersResult.status == Status::Success);


        m_renderSurface = std::move(surfaceResult.value);
        m_semaphoreImageReady = std::move(semaphoreImageResult.value);
        m_semaphoreRenderingFinished = std::move(semaphoreRenderingResult.value);
        m_swapchain = std::move(swapchainResult.value);
        m_commandPool = std::move(commandPoolResult.value);
        m_commandBuffers = std::move(commandBuffersResult.value);

        return true;
    }

#elif defined(PLATFORM_LINUX)

    Bool RendererVk::CreateSurface(IWindowSurfaceX* windowSurface)
    {
        auto surfaceResult = m_device.CreateSurface(windowSurface);
        ASSERT_RETURN(surfaceResult.status == Status::Success);
        VkSurfaceKHR& surface = surfaceResult.value.Get();

        if(!m_device.CheckAdapterSurfaceSupport(surface))
        {
            return false;
        }

        auto semaphoreImageResult = m_device.CreateSemaphore();
        ASSERT_RETURN(semaphoreImageResult.status == Status::Success);


        auto semaphoreRenderingResult = m_device.CreateSemaphore();
        ASSERT_RETURN(semaphoreRenderingResult.status == Status::Success);


        VkPresentModeKHR targetPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        if (!CheckPresentModeSupported(m_device.GetSupporedPresentModes(surface),
                                       targetPresentMode))
        {
            return false;
        }

        VkColorSpaceKHR targetColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        VkFormat targetFormat = VK_FORMAT_B8G8R8A8_UNORM;

        if (!CheckSurfaceFormatSupport(m_device.GetSupportedSurfaceFormats(surface),
                                       targetColorSpace,
                                       targetFormat))
        {
            LOGE("Could not find supported surface");
            return false;
        }

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        ASSERT_RETURN(m_device.GetSurfaceCapabilities(surface, surfaceCapabilities));

        Uint32 surfaceWidth = windowSurface->GetSurfaceExtent().width;
        Uint32 surfaceHeight = windowSurface->GetSurfaceExtent().height;

        if(surfaceWidth > surfaceCapabilities.maxImageExtent.width ||
           surfaceHeight > surfaceCapabilities.maxImageExtent.height)
        {
            LOGE("Invalid surface extents");
            return false;
        }

        if(surfaceWidth < surfaceCapabilities.minImageExtent.width ||
           surfaceHeight < surfaceCapabilities.minImageExtent.height)
        {
            LOGE("Invalid surface extents");
            return false;
        }

        Uint32 imageCount = 2;
        if(imageCount > surfaceCapabilities.maxImageCount ||
           imageCount < surfaceCapabilities.minImageCount)
        {
            LOGE("Invalid image count");
            return false;
        }

        SwapchainCreateInfo swapchainCreateInfo;
        swapchainCreateInfo.colorSpace = targetColorSpace;
        swapchainCreateInfo.surfaceFormat = targetFormat;
        swapchainCreateInfo.imagesCount = imageCount;
        swapchainCreateInfo.transformation = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapchainCreateInfo.imageWidth = surfaceWidth;
        swapchainCreateInfo.imageHeight = surfaceHeight;
        swapchainCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainCreateInfo.presentationMode = targetPresentMode;
        swapchainCreateInfo.surface = surface;

        auto swapchainResult = m_device.CreateSwapchain(swapchainCreateInfo);
        ASSERT_RETURN(swapchainResult.status == Status::Success);

        auto commandPoolResult = m_device.CreateCommandPool();
        if (commandPoolResult.status != Status::Success)
        {
            return false;
        }

        auto commandBuffersResult = m_device.AllocateCommandBuffers(swapchainResult.value.Get(),
                                                                    commandPoolResult.value.Get());
        if (commandBuffersResult.status != Status::Success)
        {
            return false;
        }

        m_renderSurface = std::move(surfaceResult.value);
        m_semaphoreImageReady = std::move(semaphoreImageResult.value);
        m_semaphoreRenderingFinished = std::move(semaphoreRenderingResult.value);
        m_swapchain = std::move(swapchainResult.value);
        m_commandPool = std::move(commandPoolResult.value);
        m_commandBuffers = std::move(commandBuffersResult.value);

        return true;
    }

#endif

    Bool RendererVk::Work()
    {
        ClearScreen();
        Present();
        return true;
    }

    Bool RendererVk::CheckPresentModeSupported(const std::vector<VkPresentModeKHR>& presentModes,
                                               VkPresentModeKHR target) const
    {
        for(auto& mode: presentModes)
        {
            if(target == mode)
            {
                return true;
            }
        }
        return false;
    }

    Bool RendererVk::CheckSurfaceFormatSupport(const std::vector<VkSurfaceFormatKHR>& surfaceFormats,
                                               VkColorSpaceKHR colorSpace,
                                               VkFormat format) const
    {
        for(auto& surface: surfaceFormats)
        {
            if(surface.colorSpace == colorSpace && surface.format == format)
            {
                    return true;
            }
        }
        return false;
    }

    void RendererVk::ClearScreen()
    {
    }

    void RendererVk::Present()
    {
    }
}
