#pragma once

#include "IWindow.hpp"
#include "Renderer.hpp"
#include "IWindowEventHandler.hpp"
#include "Types.hpp"

#include <vector>
#include <unordered_map>

namespace engine {

	class WindowManager {
	public:
		WindowManager(Renderer* renderer);
		WindowManager(WindowManager&) = delete;
		WindowManager(WindowManager&&) = delete;
		~WindowManager();

		int Initialize();
	
		IWindow* CreateWindowInstance(Uint32 x, Uint32 y, Uint32 width, Uint32 height);

		void Shutdown();
	private:
		Renderer* m_renderer;
	};

} // namespace engine
