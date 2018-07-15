#include "RendererVk.hpp"

#include "CommandBufferVk.hpp"
#include "SwapchainVk.hpp"
#include "Utility.hpp"

namespace engine
{

    Bool RendererVk::Initialize()
    {
        if(!m_device.Initialize())
        {
            return false;
        }

        m_commandPool = m_device.CreateCommandPool();
        if (!m_commandPool)
        {
            return false;
        }

        return true;
    }

    void RendererVk::Finalize()
    {
        m_device.Finalize();
    }

    void RendererVk::Submit(ObjectRef<CommandBufferVk>& commandBuffer)
    {
        m_device.SubmitQueue(commandBuffer);
    }

    ObjectRef<CommandBufferVk> RendererVk::CreateCommandBuffer()
    {
        // change it
        auto commandBuffers = m_device.AllocateCommandBuffers(1, m_commandPool.Get());
        //return ObjectRefCast<ICommandBuffer, CommandBufferVk>(commandBuffers[0]);
        return commandBuffers[0];
    }

    ObjectRef<SwapchainVk> RendererVk::GetSwapchain()
    {
        //return ObjectRefCast<ISwapchain, SwapchainVk>(m_swapchain);
        return m_swapchain;
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


        m_swapchain = m_device.CreateSwapchain(swapchainCreateInfo);
        ASSERT_RETURN(m_swapchain);

        m_commandPool = m_device.CreateCommandPool();
        ASSERT_RETURN(commandPool);

        m_commandBuffers = m_device.AllocateCommandBuffers(swapchainResult.value.Get(),
                                                                    commandPoolResult.value.Get());
        ASSERT_RETURN(commandBuffer.size() > 0);


        m_renderSurface = std::move(surfaceResult.value);
        m_semaphoreImageReady = std::move(semaphoreImageResult.value);
        m_semaphoreRenderingFinished = std::move(semaphoreRenderingResult.value);
        return true;
    }

#elif defined(PLATFORM_LINUX)

    Bool RendererVk::CreateSurface(IWindowSurfaceX* windowSurface)
    {
        m_renderSurface = m_device.CreateSurface(windowSurface);
        ASSERT_RETURN(m_renderSurface);
        VkSurfaceKHR& surface = m_renderSurface.Get();

        if(!m_device.CheckAdapterSurfaceSupport(surface))
        {
            return false;
        }

        auto semaphoreImage = m_device.CreateSemaphore();
        ASSERT_RETURN(semaphoreImage);

        auto semaphoreRendering = m_device.CreateSemaphore();
        ASSERT_RETURN(semaphoreRendering);


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

        SwapchainCreateInfo info;
        info.colorSpace = targetColorSpace;
        info.surfaceFormat = targetFormat;
        info.imagesCount = imageCount;
        info.transformation = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        info.imageWidth = surfaceWidth;
        info.imageHeight = surfaceHeight;
        info.imageCount = 2;
        info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        info.presentationMode = targetPresentMode;
        info.surface = surface;

        m_swapchain = m_device.CreateSwapchain(info);
        ASSERT_RETURN(m_swapchain);

        auto commandPool = m_device.CreateCommandPool();
        if (!commandPool)
        {
            LOGE("Invalid command pool");
            return false;
        }

        m_commandBuffers = m_device.AllocateCommandBuffers(m_swapchain->GetImageCount(),
                                                           commandPool.Get());
        if (m_commandBuffers.size() != m_swapchain->GetImageCount())
        {
            LOGE("Invalid command buffers");
            return false;
        }

        return true;
    }

#endif

    Bool RendererVk::Work()
    {
        // for now this should be good engouh
        //if (!m_swapchain->GetAvailableImageCount())
        //{
            //Status status = m_swapchain->AcquireImage();
            //if (status != Status::Success)
            //{
            //    return true;
            //}
        //}
        //CommandBuffer* commandBuffer = nullptr;
        //m_commandQueue.PopCommandBuffer(commandBuffer);
        //m_swapchain->PresentImage();
        return false;
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
