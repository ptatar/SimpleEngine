#include "RendererVk.hpp"

namespace engine
{

    Bool RendererVk::Initialize()
    {
        return m_device.Initialize(); // remove device from renderer
    }

    void RendererVk::Shutdown()
    {
        m_device.Shutdown();
    }

#if defined(PLATFORM_WINDOWS)

    // FIX ME Failure flow cleanup
    Bool RendererVk::CreateSurface(IWindowSurface32* windowSurface)
    {
        auto surfaceResult = m_device.CreateSurface(windowSurface);
        if(surfaceResult.status != Status::Success)
        {
            return false;
        }

        auto semaphoreImageResult = m_device.CreateSemaphore();
        if(semaphoreImageResult.status != Status::Success)
        {
            return false;
        }

        auto semaphoreRenderingResult = m_device.CreateSemaphore();
        if(semaphoreRenderingResult.status != Status::Success)
        {
            return false;
        }

        if(m_device.CreateSwapChain() != true)
        {
            return false;
        }

        m_renderSurface = std::move(result.value);
        m_semaphoreImageReady = std::move(semaphoreImageResult.value);
        m_semaphoreRenderingFinished = std::move(semaphoreRenderingResult.value);

        return true;
    }

#elif defined(PLATFORM_LINUX)

    Bool RendererVk::CreateSurface(IWindowSurfaceX* windowSurface)
    {
        auto surfaceResult = m_device.CreateSurface(windowSurface);
        if(surfaceResult.status != Status::Success)
        {
            return false;
        }

        auto semaphoreImageResult = m_device.CreateSemaphore();
        if(semaphoreImageResult.status != Status::Success)
        {
            return false;
        }

        auto semaphoreRenderingResult = m_device.CreateSemaphore();
        if(semaphoreRenderingResult.status != Status::Success)
        {
            return false;
        }

        SwapchainCreateInfo swapchainCreateInfo;
        swapchainCreateInfo.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        swapchainCreateInfo.surfaceFormat = VK_FORMAT_B8G8R8A8_UNORM;
        swapchainCreateInfo.imagesCount = 2;
        swapchainCreateInfo.transformation = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapchainCreateInfo.imageWidth = windowSurface->GetSurfaceExtent().width;
        swapchainCreateInfo.imageHeight = windowSurface->GetSurfaceExtent().height;
        swapchainCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainCreateInfo.presentationMode = VK_PRESENT_MODE_MAILBOX_KHR;
        swapchainCreateInfo.surface = surfaceResult.value.Get();


        auto swapchainResult = m_device.CreateSwapchain(swapchainCreateInfo);
        if(swapchainResult.status != Status::Success)
        {
            return false;
        }

        m_renderSurface = std::move(surfaceResult.value);
        m_semaphoreImageReady = std::move(semaphoreImageResult.value);
        m_semaphoreRenderingFinished = std::move(semaphoreRenderingResult.value);
        return true;
    }

#endif

}
