#pragma once

#include "Types.hpp"
#include "DeviceVk.hpp"
#include "IRenderer.hpp"
#include "WindowSurface.hpp"
#include "IJob.hpp"

namespace engine
{
    class CommandRecorder: public IJob
    {
    public:
        CommandRecorder();
        ~CommandRecorder();
    }

    class RendererVk: public IRenderer, public IJob
    {
    public:
        RendererVk() {}
        ~RendererVk() {}

        Bool Initialize() override;
        void Shutdown() override;
        void Update() override;
    #if defined(PLATFORM_WINDOWS)
        Bool CreateSurface(IWindowSurface32* windowSurface) override;
    #elif defined(PLATFORM_LINUX)
        Bool CreateSurface(IWindowSurfaceX* windowSurface) override;
    #endif
    virtual Bool Work() override;

    private:
        DeviceVk m_device;
        SurfaceHandler m_renderSurface;
        SemaphoreHandler m_semaphoreImageReady;
        SemaphoreHandler m_semaphoreRenderingFinished;
        SwapchainHandler m_swapchain;
        CommandPoolHandler m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
    };
} // namespace engine
