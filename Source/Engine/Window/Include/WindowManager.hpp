#pragma once

#include "IWindow.hpp"
#include "RendererManager.hpp"
#include "IWindowEventHandler.hpp"
#include "Types.hpp"

#include <vector>
#include <unordered_map>

namespace engine {

	class WindowManager {
	public:
		WindowManager(RendererManager* rendererManager);
		WindowManager(WindowManager&) = delete;
		WindowManager(WindowManager&&) = delete;
		~WindowManager();

		int Initialize();
	
		IWindow* CreateWindowInstance(Uint32 x, Uint32 y, Uint32 width, Uint32 height);
		void Shutdown();
	private:
		RendererManager* m_rendererManager;
	};

} // namespace engine
