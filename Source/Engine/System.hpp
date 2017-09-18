#pragma once

#include "ThreadManager.hpp"
#include "WindowManager.hpp"
#include "RendererManager.hpp"

namespace engine
{

    class System
    {
    public:
        System() {}
        ~System() {}
        Bool Initialize();
        void MainLoop();
        void Shutdown();

    public:
        ThreadManager m_threadManager;
        WindowManager m_windowManager;
        RendererManager m_renderManager;
    };

} // namespace engine