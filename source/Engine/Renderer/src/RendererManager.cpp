#include "RendererManager.hpp"

#include "RendererVk.hpp"

namespace engine
{
	IRenderer* RendererManager::CreateRenderer()
	{
		return new RendererVk();
	}
} // namespace engine