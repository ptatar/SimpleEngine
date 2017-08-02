#include "RendererVk.hpp"

namespace engine
{

	engine::Bool RendererVk::Initialize()
	{
		return m_device.Initialize();
	}

	void RendererVk::Shutdown()
	{
		m_device->
	}

	Bool RendererVk::CreateSurface32(Window32* window32)
	{
		m_device->CreateSurface32(window32);
	}

	Bool RendererVk::CreateSurfaceX(WindowX* windowX)
	{
		m_device->CreateSurfaceX(windowX);
	}
}