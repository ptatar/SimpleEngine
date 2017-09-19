#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <fstream>

#include "System.hpp"
#include "Logger.hpp"

using namespace engine;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //Attach console for debug
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    System system;
    if (!system.Initialize())
    {
        return -1;
    }
    system.MainLoop();
    system.Shutdown();
    ThreadManager threadManager;
    if (!threadManager.Initialize())
    {
        return -1;
    }


    ObjectRef<IWindow> window = winMag.CreateWindowInstance(0, 0, 800, 600);
    if (!window)
    {
        return -1;
    }

    if (window != nullptr) {
        window->Show();
        while(window->HandleEvents()) {
        }
    }
    else {

    }
    window->Shutdown();
    delete window;
    return 0;
}
