#pragma once

#include "Types.hpp"
#include "DeviceVk.hpp"
#include "IRenderer.hpp"
#include "WindowSurface.hpp"
#include "IJob.hpp"
#include "Utility.hpp"
#include "SwapchainVk.hpp"
#include "CommandBufferVk.hpp"

#include <deque>
#include <mutex>

namespace engine
{
    //class CommandBufferVk;
    //class SwapchainVk;


    class CommandRecorder: public IJob
    {
    public:
        CommandRecorder();
        ~CommandRecorder();
    };

    class RendererVk: /*public IRenderer,*/ public ISurfaceEventListener, public IJob
    {
    public:
        RendererVk() {}

        ~RendererVk() {}

        Bool Initialize();

        void Finalize();

        void Submit(ObjectRef<CommandBufferVk>& commandBuffer);

        ObjectRef<CommandBufferVk> CreateCommandBuffer();

        ObjectRef<SwapchainVk> GetSwapchain();

    #if defined(PLATFORM_WINDOWS)
        Bool CreateSurface(IWindowSurface32* windowSurface) override;
    #elif defined(PLATFORM_LINUX)
        Bool CreateSurface(IWindowSurfaceX* windowSurface);
    #endif
        Bool Work() override;

        //void PushCommandBuffer(CommandBuffer& commandBuffer);

    private:
        Bool CheckPresentModeSupported(const std::vector<VkPresentModeKHR>& presentModes,
                                       VkPresentModeKHR target) const;

        Bool CheckSurfaceFormatSupport(const std::vector<VkSurfaceFormatKHR>& surfaceFormats,
                                      VkColorSpaceKHR colorSpace,
                                      VkFormat format) const;

        // ISurfaceEventListener
        virtual void OnResize(Uint32 width, Uint32 height) override {}

        virtual void OnReposition(Uint32 x, Uint32 y) override {}

        virtual void OnShow() override {}

        virtual void OnHide() override {}

        virtual void OnFullscreen() override {}

        virtual void OnShutdown() override { /*Shutdown();*/ }

        void ClearScreen();

        void Present();

        //Status PopCommandBuffer(CommandBuffer& commandBuffer);

    private:
        DeviceVk m_device;

        SurfaceG m_renderSurface;

        SemaphoreG m_semaphoreImageReady;

        SemaphoreG m_semaphoreRenderingFinished;

        CommandPoolG m_commandPool;

        ObjectRef<SwapchainVk> m_swapchain;

        std::vector<ObjectRef<CommandBufferVk>> m_commandBuffers;

        TimeUnits m_swapchainTimeout;
    };
} // namespace engine
