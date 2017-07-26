#pragma once

#include "IRenderer.hpp"

namespace engine
{
	class RendererManager
	{
	public:
		RendererManager() {}
		~RendererManager() {}
		IRenderer* CreateRenderer();
	};
} // namespace engine