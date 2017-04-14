#pragma once
#include "IWindow.hpp"
#include "IWindowEventHandler.hpp"

#include <vector>
#include <unordered_map>

namespace engine {

class WindowManager {
public:
	WindowManager();
	WindowManager(WindowManager&) = delete;
	WindowManager(WindowManager&&) = delete;
	~WindowManager();

	int Initialize();
	
	IWindow* CreateWindowInstance(unsigned x, unsigned y, unsigned width, unsigned height);

	void Shutdown();
private:
	std::vector<IWindow*> m_windows;
};

} // namespace engine
