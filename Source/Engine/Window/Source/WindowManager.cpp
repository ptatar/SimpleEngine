#include "WindowManager.hpp"

#include "Logger.hpp"

#if defined(PLATFORM_WINDOWS)
    #include "Window32.hpp"
#elif defined(PLATFORM_LINUX)
    #include "WindowX.hpp"
#endif
#include "IWindow.hpp"

namespace engine {

    WindowManager::WindowManager(RendererManager* rendererManager, ThreadManager* threadManager)
        : m_rendererManager(rendererManager)
        , m_threadManager(threadManager) {}


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

#if defined(PLATFORM_WINDOW)
    ObjectRef<IWindow> WindowManager::CreateWindowInstance(Uint32 x,
                                                           Uint32 y,
                                                           Uint32 width,
                                                           Uint32 height,
                                                           Bool mainWindow)
    {
        ObjectRef<Window32> window(new Window32());

        int res = window->Initialize(x, y, width, height);
        if (res != 0)
        {
            return nullptr;
        }
        m_threadManager->Execute(static_cast<ObjectRef<IJob>>(window));
        return static_cast<ObjectRef>>(window);
    }

#elif defined(PLATFORM_LINUX)
    ObjectRef<IWindow> WindowManager::CreateWindowInstance(Uint32 x,
                                                           Uint32 y,
                                                           Uint32 width,
                                                           Uint32 height)
    {
        ObjectRef<WindowX> window(new WindowX());

        int res = window->Initialize(x, y, width, height);
        if (res != 0)
        {
            return nullptr;
        }

        auto job = static_cast<ObjectRef<IJob>>(window);
        m_threadManager->Execute(job);
        return ObjectRefCast<IWindow, WindowX>(window);
    }
#endif

} // namespace engine
