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

	private:
		DeviceVk m_device;
	};
} // namespace engine