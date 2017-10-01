#pragma once

#include "IRenderer.hpp"
#include "WindowSurface.hpp"

#include <memory>

namespace engine
{
    class RendererManager
    {
    public:
        RendererManager() {}
        ~RendererManager() {}
#if defined(PLATFORM_WINDOWS)
        std::unique_ptr<IRenderer> CreateRenderer(IWindowSurface32* surface);
#elif defined(PLATFORM_LINUX)
        std::unique_ptr<IRenderer> CreateRenderer(IWindowSurfaceX* surface);
#endif
    };

    class RendererManagerUnit: public IUnit, public RendererManager
    {
    public:
        RendererManagerUnit(ISystem* system)
            : IUnit(system) {}
    }
} // namespace engine
