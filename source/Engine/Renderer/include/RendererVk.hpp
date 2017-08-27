#pragma once

#include "Types.hpp"
#include "DeviceVk.hpp"
#include "IRenderer.hpp"
#include "WindowSurface.hpp"

namespace engine
{
    class RendererVk: public IRenderer
    {
    public:
        RendererVk() {}
        ~RendererVk() {}

        Bool Initialize() override;
        void Shutdown() override;

    #if defined(PLATFORM_WINDOWS)
        Bool CreateSurface(IWindowSurface32* windowSurface) override;
    #elif defined(PLATFORM_LINUX)
        Bool CreateSurface(IWindowSurfaceX* windowSurface) override;
    #endif

    private:
        DeviceVk m_device;
        SurfaceHandler m_renderSurface;
        SemaphoreHandler m_semaphoreImageReady;
        SemaphoreHandler m_semaphoreRenderingFinished;
        SwapchainHandler m_swapchain;
    };
} // namespace engine
