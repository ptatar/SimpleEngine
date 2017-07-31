#include "WindowManager.hpp"

#include "Logger.hpp"

#if defined(PLATFORM_WINDOWS)
    #include "Window32.hpp"
#elif defined(PLATFORM_LINUX)
    #include "WindowX.hpp"
#endif

namespace engine {

WindowManager::WindowManager(IRenderer* renderer): m_renderer(renderer)  {}


WindowManager::~WindowManager() {
    Shutdown();
}


int WindowManager::Initialize() {
    return 0;
}


void WindowManager::Shutdown() {
}


IWindow* WindowManager::CreateWindowInstance(Uint32 x, Uint32 y, Uint32 width, Uint32 height) {
#if defined(PLATFORM_WINDOWS)
    Window32* window = new Window32();
#elif defined (PLATFORM_LINUX)
    WindowX* window = new WindowX();
#else
	IWindow* window = nullptr;
    LOGE("Unimplemented!");
    return nullptr;
#endif

    int res = window->Initialize(x, y, width, height);
    if (res != 0)
	{
        delete window;
        return nullptr;
    }

	//if (window->RequestRendererSurface(m_renderer))
	//{
	//	delete window;
	//	return nullptr;
	//}

    return window;
}


} // namespace engine
