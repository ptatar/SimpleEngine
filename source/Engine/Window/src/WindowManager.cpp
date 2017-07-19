#include "WindowManager.hpp"

#include "Logger.hpp"

#if defined(PLATFORM_WINDOWS)
    #include "Window32.hpp"
#elif defined(PLATFORM_LINUX)
    #include "WindowX.hpp"
#endif

namespace engine {

WindowManager::WindowManager() {}


WindowManager::~WindowManager() {
    Shutdown();
}


int WindowManager::Initialize() {
    return 0;
}


void WindowManager::Shutdown() {
    for (auto* window: m_windows) {
        if (window != nullptr) {
            window->Shutdown();
            window = nullptr;
        }
    }
}


IWindow* WindowManager::CreateWindowInstance(unsigned x, unsigned y, unsigned width, unsigned height) {
    IWindow* window = nullptr;
#if defined(PLATFORM_WINDOWS)
    window = new Window32();
#elif defined (PLATFOM_LINUX)
    window = new WindowX();
#else
    LOGE("Unimplemented!");
    return nullptr;
#endif
    int res = window->Initialize(x, y, width, height);
    if (res != 0) {
        delete window;
        return nullptr;
    }

    m_windows.push_back(window);
    return window;
}


} // namespace engine
