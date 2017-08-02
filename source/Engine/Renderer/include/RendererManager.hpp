#pragma once

#include "IRenderer.hpp"

namespace engine
{
	class Surface32
	{

	};

	class RendererManager
	{
	public:
		RendererManager() {}
		~RendererManager() {}
#if defined(PLATFORM_WINDOWS)
		IRenderer* CreateRenderer(Window32* window32);
#elif defined(PLATFORM_LINUX)
		IRenderer* CreateRenderer(WindowX* windowX);
#endif
	};
} // namespace engine