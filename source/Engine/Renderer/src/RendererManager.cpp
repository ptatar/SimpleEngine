#include "RendererManager.hpp"

#include "RendererVk.hpp"

namespace engine
{
#if defined(PLATFORM_WINDOWS)
	std::unique_ptr<IRenderer> RendererManager::CreateRenderer(IWindowSurface32* surface)
	{
		std::unique_ptr<RendererVk> renderer = std::make_unique<RendererVk>();
		if (!renderer->Initialize())
		{
			return nullptr;
		}
		if (!renderer->CreateSurface(surface))
		{
			return nullptr;
		}
		return renderer;
	}
#elif defined(PLATFORM_LINUX)
	std::unique_ptr<IRenderer> RendererManager::CreateRenderer(IWindowSurfaceX* surface)
	{
		RendererVk* renderer = new RendererVk();
		renderer->CreateSurface(surface);
		return renderer;
	}
#endif
} // namespace engine