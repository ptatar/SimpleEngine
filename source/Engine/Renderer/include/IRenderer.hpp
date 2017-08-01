#pragma once

#include "Types.hpp"

namespace engine {
	class 
	class IRenderer
	{
	public:
		virtual ~IRenderer() {}
		virtual Bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual Surface CreateSurface32() {};
		virtual Surface CreateSurfaceX() {};
	};
} // namespace engine