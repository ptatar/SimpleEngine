#pragma once

#include "Types.hpp"
#include "DeviceVk.hpp"
#include "IRenderer.hpp"

namespace engine
{
	class RendererVk: public IRenderer
	{
	public:
		RendererVk() {}
		~RendererVk() {}

		Bool Initialize() override;
		void Shutdown() override;

		Bool CreateSurface32(Window32* window32);
		Bool CreateSurfaceX(WindowX* windowX);

	private:
		DeviceVk m_device;
	};
} // namespace engine