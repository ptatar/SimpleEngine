#pragma once

#include "IWindow.hpp"
#include "RendererManager.hpp"
#include "IWindowEventHandler.hpp"
#include "Types.hpp"

#include <vector>
#include <unordered_map>

namespace engine {

    class WindowManager {
    public:
        WindowManager(RendererManager* rendererManager);
        WindowManager(WindowManager&) = delete;
        WindowManager(WindowManager&&) = delete;
        ~WindowManager();

        Bool Initialize();
        void Update();
        IWindow* CreateWindowInstance(Uint32 x, Uint32 y, Uint32 width, Uint32 height);
        void Shutdown();
    private:
        RendererManager* m_rendererManager;
        std::vector<std::unique_ptr<IWindow>> m_windows;
    };

} // namespace engine
