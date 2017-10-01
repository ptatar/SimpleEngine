#include "System.hpp"

using namespace engine;

int main()
{
    System system;
    if(!system.Initialize())
    {
        return -1;
    }
    WindowManager& windowManager = system.m_windowManager;
    auto window = windowManager.CreateWindowInstance(0, 0, 800, 600);
    system.MainLoop();
    system.Shutdown();
    /*RendererManager rendererManager;
    WindowManager windowManager(&rendererManager);
    window->Show();
    while(window->HandleEvents()) {}
    window->Shutdown();
    */
    return 0;
}
