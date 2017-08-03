#include "RendererVk.hpp"

namespace engine
{

	engine::Bool RendererVk::Initialize()
	{
		return m_device.Initialize(); // remove device from renderer
	}

	void RendererVk::Shutdown()
	{
		m_device.Shutdown();
	}

#if defined(PLATFORM_WINDOWS)

	Bool RendererVk::CreateSurface(IWindowSurface32* windowSurface)
	{
		DeviceVk::Result<VkSurfaceKHR> result = m_device.CreateSurface(windowSurface);
		if (result.status == DeviceVk::Status::Success)
		{
			m_renderSurface = result.value;
			return true;
		}
		else
		{
			
			return false;
		}
	}

#elif defined(PLATFORM_LINUX)

	Bool RendererVk::CreateSurface(IWindowSurfaceX* windowSurface)
	{
		m_device->CreateSurface(surface);
	}

#endif

}