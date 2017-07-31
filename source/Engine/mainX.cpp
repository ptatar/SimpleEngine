#include "WindowManager.hpp"
#include "RendererManager.hpp"

using namespace engine;

int main()
{
    RendererManager rendererManager;
    IRenderer* renderer =rendererManager.CreateRenderer();
    WindowManager windowManager(renderer);
    IWindow* window = windowManager.CreateWindowInstance(0, 0, 800, 600);
    window->Show();
    while(window->HandleEvents()) {}
    window->Shutdown();
    renderer->Shutdown();
    return 0;
}
