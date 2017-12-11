#include "ThreadManager.hpp"
#include "WindowManager.hpp"
#include "RendererManager.hpp"

#include "Utility.hpp"

using namespace engine;

int main()
{
    ThreadManager threadManager;
    threadManager.Initialize(2, 4);
    RendererManager rendererManager(&threadManager);
    WindowManager windowManager(&rendererManager, &threadManager);
    auto window = windowManager.CreateWindowInstance(0, 0, 800, 600);
    auto renderer = rendererManager.GetRenderer(window->GetSurface());
    window->Show();
    //Sleep(TimeUnits::MakeSeconds(10));
    //window->Shutdown();
    threadManager.Finish();
    //system.MainLoop();
    //system.Shutdown();
    /*RendererManager rendererManager;
    WindowManager windowManager(&rendererManager);
    window->Show();
    while(window->HandleEvents()) {}
    window->Shutdown();
    */
    return 0;
}
