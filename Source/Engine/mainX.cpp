#include "ThreadManager.hpp"
#include "WindowManager.hpp"
#include "RendererManager.hpp"

#include "Utility.hpp"

#include "RendererVk.hpp"

using namespace engine;

int main()
{
    ThreadManager threadManager;
    threadManager.Initialize(2, 4);
    RendererManager rendererManager(&threadManager);
    WindowManager windowManager(&rendererManager, &threadManager);
    auto window = windowManager.CreateWindowInstance(0, 0, 800, 600);
    auto renderer = rendererManager.GetRenderer(window->GetSurface());
    if (!renderer)
    {
        return 1;
    }
    window->Show();
    //Sleep(TimeUnits::MakeSeconds(10));
    //window->Shutdown();
    auto commandBuffer = renderer->CreateCommandBuffer();
    auto swapchain = renderer->GetSwapchain();
    Uint32 imageCount = swapchain->GetImageCount();
    RingBuffer<ObjectRef<CommandBufferVk>> commandRingBuffer(imageCount);
    for (Uint32 i = 0; i < imageCount; i++)
    {
        ObjectRef<CommandBufferVk>& commandBuffer = commandRingBuffer.GetNext();
        commandBuffer = renderer->CreateCommandBuffer();
        if (!commandBuffer)
        {
            // TODO shutdown threads
            return 1;
        }
    }

    while(!threadManager.IsFinished())
    {
        auto& commandBuffer = commandRingBuffer.GetNext();
        swapchain->AcquireImage();
        if(commandBuffer->Wait(TimeUnits::MakeSeconds(1)) != Status::Success)
        {
            // TODO shutdown threads
            return 1;
        }
        commandBuffer->Begin();
        commandBuffer->Clear(swapchain->GetCurrentImage(), ImageAspect::Color);
        commandBuffer->PrepereForPresent(swapchain->GetCurrentImage());
        commandBuffer->End();
        renderer->Submit(commandBuffer);
        swapchain->PresentImage(commandBuffer);
    }
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
