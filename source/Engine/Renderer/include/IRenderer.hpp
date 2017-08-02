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

//		virtual Bool CreateSurface32() {};
//		virtual Bool CreateSurfaceX() {};
	};
} // namespace engine