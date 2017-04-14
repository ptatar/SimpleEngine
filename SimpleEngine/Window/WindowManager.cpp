#include "WindowManager.hpp"
#include "Window32.hpp"

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
#ifdef _WIN32 || _WIN64
	IWindow* window = new Window32();
#elif
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