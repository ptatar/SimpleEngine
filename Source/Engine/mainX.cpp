#include "System.hpp"

using namespace engine;

int main()
{
    RendererManager rendererManager;
    WindowManager windowManager(&rendererManager);
    auto window = windowManager.CreateWindowInstance(0, 0, 800, 600);
    window->Show();
    while(window->HandleEvents()) {}
    window->Shutdown();
    return 0;
}
