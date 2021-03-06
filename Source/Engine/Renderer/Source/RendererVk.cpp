#include "RendererVk.hpp"

#include "CommandBufferVk.hpp"
#include "SwapchainVk.hpp"
#include "Utility.hpp"

#include "FileManager.hpp"

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

        if (!m_shaderManager->Initialize(&m_device))
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
        ASSERT(m_renderSurface);
        VkSurfaceKHR& surface = m_renderSurface.Get();

        if(!m_device.CheckAdapterSurfaceSupport(surface))
        {
            return false;
        }

        auto semaphoreImage = m_device.CreateSemaphore();
        ASSERT(semaphoreImage);

        auto semaphoreRendering = m_device.CreateSemaphore();
        ASSERT(semaphoreRendering);

        Bool found = false;
        VkFormat targetFormat = VK_FORMAT_B8G8R8A8_UNORM;
        VkColorSpaceKHR targetSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        std::vector<VkSurfaceFormatKHR> supportedFormats = m_device.GetSupportedSurfaceFormats(surface);
        for(int i = 0; i < supportedFormats.size(); ++i)
        {
           if (supportedFormats[i].format == targetFormat && supportedFormats[i].colorSpace == targetSpace)
           {
               found = true;
           }
        }

        if (found == false)
        {
            LOGE("Target format and color space not supported: fmt: %d cs:", targetFormat, targetSpace);
        }

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        if (!m_device.GetSurfaceCapabilities(surface, surfaceCapabilities))
        {
            return false;
        }

        Uint32 targetImageCount = 2;
        Uint32 targetWidth = windowSurface->GetSurfaceExtent().width;
        Uint32 targetHeight = windowSurface->GetSurfaceExtent().height;

        if (targetImageCount < surfaceCapabilities.minImageCount ||
            targetImageCount > surfaceCapabilities.maxImageCount)
        {
            LOGE("Unsupported image count: %d", targetImageCount);
            return false;
        }

        if (targetWidth < surfaceCapabilities.minImageExtent.width ||
            targetWidth > surfaceCapabilities.maxImageExtent.width ||
            targetHeight < surfaceCapabilities.minImageExtent.height ||
            targetHeight > surfaceCapabilities.maxImageExtent.height)
        {
            LOGE("Unsupported image extents: w:%d h:%d", targetWidth, targetHeight);
            return false;
        }

        SwapchainCreateInfo info;
        info.surfaceFormat = targetFormat;
        info.imageCount = targetImageCount;
        info.imageWidth = windowSurface->GetSurfaceExtent().width;
        info.imageHeight = windowSurface->GetSurfaceExtent().height;
        info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        info.presentationMode = VK_PRESENT_MODE_FIFO_KHR;
        info.surface = surface;
        info.colorSpace = targetSpace;
        info.transformation = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

        m_swapchain = m_device.CreateSwapchain(info);
        ASSERT(m_swapchain);

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


    void RendererVk::ClearScreen()
    {
    }

    void RendererVk::Present()
    {
    }
}
