#pragma once

#include "IRenderer.hpp"
#include "WindowSurface.hpp"
#include "ThreadManager.hpp"

namespace engine
{
    class RendererManager
    {
    public:
        RendererManager(ThreadManager* threadManager): m_threadManager(threadManager) {}
        ~RendererManager() {}

        ObjectRef<IRenderer> GetRenderer(IWindowSurface* surface);

    private:
        ThreadManager* m_threadManager;
    };
/*
    class RendererManagerUnit: public IUnit, public RendererManager
    {
    public:
        RendererManagerUnit(ISystem* system)
            : IUnit(system) {}
    }
*/
} // namespace engine
