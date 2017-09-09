#include "WindowManager.hpp"

#include "Logger.hpp"

#if defined(PLATFORM_WINDOWS)
    #include "Window32.hpp"
#elif defined(PLATFORM_LINUX)
    #include "WindowX.hpp"
#endif

namespace engine {

WindowManager::WindowManager(RendererManager* rendererManager): m_rendererManager(rendererManager)  {}


WindowManager::~WindowManager() {
    Shutdown();
}


int WindowManager::Initialize() {
    return 0;
}


void WindowManager::Shutdown() {
}


IWindow* WindowManager::CreateWindowInstance(Uint32 x, Uint32 y, Uint32 width, Uint32 height) {
	//clean this up
#if defined(PLATFORM_WINDOWS)
    std::unique_ptr<Window32> window = std::make_unique<Window32>();
#elif defined (PLATFORM_LINUX)
    std::unique_ptr<WindowX> window = std::make_unique<WindowX>();
#else
	ERROR("Dead code path")
#endif
	
    int res = window->Initialize(x, y, width, height);
    if (res != 0)
	{
        return nullptr;
    }
	std::unique_ptr<IRenderer> renderer(m_rendererManager->CreateRenderer(window.get()));
	if (renderer == nullptr)
	{
		return nullptr;
	}
	window->BindRenderer(std::move(renderer));

	return window.release();
}


} // namespace engine
