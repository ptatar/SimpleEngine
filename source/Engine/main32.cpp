#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <fstream>

#include "WindowManager.hpp"
#include "Renderer.hpp"
#include "common/Logger.hpp"

using namespace engine;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//Attach console for debug
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	WindowManager winMag;
	winMag.Initialize();
	IWindow* window = winMag.CreateWindowInstance(0, 0, 800, 600);
	Renderer renderer;
	if (!renderer.Initialize(window)) {
		return 1;
	}

	if (window != nullptr) {
		window->Show();
		while(window->HandleEvents()) {
		}
	}
	else {

	}
	renderer.Uninitialize();
	window->Shutdown();
	return 0;
}
