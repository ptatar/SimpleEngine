#pragma once

#include "ThreadManager.hpp"
#include "WindowManager.hpp"
#include "RendererManager.hpp"

namespace engine
{

    class System: public ISystem
    {
    public:
        System();
        ~System();
        Bool Initialize();
        void MainLoop();
        void Shutdown();

    public:
        ThreadManagerUnit m_threadManager;
        WindowManagerUnit m_windowManager;
        RendererManagerUnit m_renderManager;
    };

} // namespace engine
