#include "RendererVk.hpp"

namespace engine
{

	engine::Bool RendererVk::Initialize()
	{
		return m_device.Initialize();
	}

	void RendererVk::Shutdown()
	{
	}

}