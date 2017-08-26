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
		DeviceVk::Result<VkSurfaceKHR> surfaceResult = m_device.CreateSurface(windowSurface);
		if(surfaceResult.status != Status::Success)
		{
			return false;
		}

        DeviceVk::Result<VkSemaphore> semaphoreImageResult = m_device.CreateSemaphore();
        if(semaphoreImageResult.status != Status::Success)
        {
            return false;
        }

        DeviceVk::Result<VkSemaphore> semaphoreRenderingResult = m_device.CreateSemaphore();
        if(semaphoreRenderingResult.status != Status::Success)
        {
            return false;
        }

        if(m_device.CreateSwapChain() != true)
        {
            return false;
        }

		m_renderSurface = result.value;
        m_semaphoreImageReady = semaphoreImageResult.value;
        m_semaphoreRenderingFinished = semaphoreRenderingResult.result;

        return false;
	}

#elif defined(PLATFORM_LINUX)

	Bool RendererVk::CreateSurface(IWindowSurfaceX* windowSurface)
	{
		Result<VkSurfaceKHR> surfaceResult = m_device.CreateSurface(windowSurface);
		if(surfaceResult.status != Status::Success)
		{
			return false;
		}

        Result<VkSemaphore> semaphoreImageResult = m_device.CreateSemaphore();
        if(semaphoreImageResult.status != Status::Success)
        {
            return false;
        }

        Result<VkSemaphore> semaphoreRenderingResult = m_device.CreateSemaphore();
        if(semaphoreRenderingResult.status != Status::Success)
        {
            return false;
        }

        if(m_device.CreateSwapchain(surfaceResult.value) != true)
        {
            return false;
        }

		m_renderSurface = surfaceResult.value;
        m_semaphoreImageReady = semaphoreImageResult.value;
        m_semaphoreRenderingFinished = semaphoreRenderingResult.value;
	}

#endif

}
