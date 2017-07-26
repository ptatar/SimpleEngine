#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <fstream>

#include "WindowManager.hpp"
#include "RendererManager.hpp"
#include "Logger.hpp"

using namespace engine;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//Attach console for debug
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	RendererManager rendererManager;
	IRenderer* renderer = rendererManager.CreateRenderer();
	if (!renderer->Initialize())
	{
		return -1;
	}
	WindowManager winMag(renderer);
	if (winMag.Initialize())
	{
		return -1;
	}
	IWindow* window = winMag.CreateWindowInstance(0, 0, 800, 600);
//	Renderer renderer;
//	if (!renderer.Initialize(window)) {
//		return 1;
//	}

	if (window != nullptr) {
		window->Show();
		while(window->HandleEvents()) {
		}
	}
	else {

	}
	renderer->Shutdown();
	window->Shutdown();
	delete renderer;
	delete window;
	return 0;
}
