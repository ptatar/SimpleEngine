#pragma once
#include "IWindow.hpp"
#include "IWindowEventHandler.hpp"
#include "Types.hpp"

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
	
	IWindow* CreateWindowInstance(Uint32 x, Uint32 y, Uint32 width, Uint32 height);

	void Shutdown();
private:
	std::vector<IWindow*> m_windows;
};

} // namespace engine
