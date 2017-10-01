#include "WindowManager.hpp"

#include "Logger.hpp"

#if defined(PLATFORM_WINDOWS)
    #include "Window32.hpp"
#elif defined(PLATFORM_LINUX)
    #include "WindowX.hpp"
#endif
#include "IWindow.hpp"

namespace engine {

    WindowManager::WindowManager(RendererManager* rendererManager): m_rendererManager(rendererManager)  {}


    WindowManager::~WindowManager()
    {
        Shutdown();
    }


    Bool WindowManager::Initialize()
    {
        return 0;
    }

    void WindowManager::Update()
    {
    }

    void WindowManager::Shutdown()
    {
    }


    ObjectRef<IWindow> WindowManager::CreateWindowInstance(Uint32 x,
                                                           Uint32 y,
                                                           Uint32 width,
                                                           Uint32 height,
                                                           Bool mainWindow)
    {
        //clean this up
    #if defined(PLATFORM_WINDOWS)
        ObjectRef<Window32> window(new Window32());
    #elif defined (PLATFORM_LINUX)
        ObjectRef<WindowX> window( new WindowX());
    #else
        ERROR("Dead code path")
    #endif

        int res = window->Initialize(x, y, width, height);
        if (res != 0)
        {
            return nullptr;
        }

        std::unique_ptr<IRenderer> renderer(m_rendererManager->CreateRenderer(window.Get()));
        if (renderer == nullptr)
        {
            return nullptr;
        }
        window->BindRenderer(std::move(renderer));
        
        return static_cast<ObjectRef<IWindow>>(window);
    }

} // namespace engine
