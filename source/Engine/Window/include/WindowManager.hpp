#pragma once

#include "IWindow.hpp"
#include "IRenderer.hpp"
#include "IWindowEventHandler.hpp"
#include "Types.hpp"

#include <vector>
#include <unordered_map>

namespace engine {

	class WindowManager {
	public:
		WindowManager(IRenderer* renderer);
		WindowManager(WindowManager&) = delete;
		WindowManager(WindowManager&&) = delete;
		~WindowManager();

		int Initialize();
	
		IWindow* CreateWindowInstance(Uint32 x, Uint32 y, Uint32 width, Uint32 height);

		void Shutdown();
	private:
		IRenderer* m_renderer;
	};

} // namespace engine
