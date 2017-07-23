#include "WindowManager.hpp"
#include "Renderer.hpp"

using namespace engine;

int main()
{
    Renderer renderer;
    WindowManager windowManager(&renderer);
    windowManager.CreateWindowInstance(0, 0, 800, 600);
    return 0;
}
