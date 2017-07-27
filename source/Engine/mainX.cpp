#include "WindowManager.hpp"
#include "RendererManager.hpp"

using namespace engine;

int main()
{
    RendererManager rendererManager;
    IRenderer* renderer =rendererManager.CreateRenderer();
    WindowManager windowManager(renderer);
    windowManager.CreateWindowInstance(0, 0, 800, 600);
    return 0;
}
