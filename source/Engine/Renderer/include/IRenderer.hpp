#pragma once

#include "Types.hpp"
#include "WindowSurface.hpp"

namespace engine {

    class IRenderer
    {
    public:
        virtual ~IRenderer() {}
        virtual Bool Initialize() = 0;
        virtual void Shutdown() = 0;
	#if defined(PLATFORM_WINDOWS)
        virtual Bool CreateSurface(IWindowSurface32* windowSurface) = 0;
	#elif defined(PLATFORM_LINUX)
        virtual Bool CreateSurface(IWindowSurfaceX* windowSurface) = 0;
	#endif
    };
} // namespace engine
