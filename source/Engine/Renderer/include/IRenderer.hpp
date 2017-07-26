#pragma once

#include "Types.hpp"

namespace engine {
	class IRenderer
	{
	public:
		virtual ~IRenderer() {}
		virtual Bool Initialize() = 0;
		virtual void Shutdown() = 0;
	};
} // namespace engine