#include "RendererManager.hpp"

#include "RendererVk.hpp"

namespace engine
{
#if defined(PLATFORM_WINDOWS)
	IRenderer* RendererManager::CreateRenderer32(Window32* window32)
	{
		RendererVk* renderer = new RendererVk();
		renderer->CreateSurface32(window32)
		return new RendererVk();
	}
#elif defined(PLATFORM_LINUX)
	IRenderer* RendererManager::CreateRendererX(WindowX* windowX)
	{
		RendererVk* renderer = new RendererVk();
		renderer->CreateSurfaceX(windowX);
		return renderer;
	}
#endif
} // namespace engine