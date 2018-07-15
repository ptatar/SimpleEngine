#pragma once

#include "ICommandBuffer.hpp"
#include "VulkanUtility.hpp"
#include "Handle.hpp"

namespace engine
{

    class DeviceVk;
    class SwapchainVk;
    class ImageVk;
    class TimeUnits;

    class CommandBufferVk//: public ICommandBuffer
    {
        enum class State
        {
            Initialized,
            Recording,
            Ready,
            Submitted,
        };
        public:
            CommandBufferVk(DeviceVk* device, VkCommandBuffer commandBuffer, SemaphoreG& semaphore, FenceG& fence)
                : m_device(device)
                , m_state(State::Initialized)
                , m_commandBuffer(commandBuffer)
                , m_semaphore(std::move(semaphore))
                , m_fence(std::move(fence)) {}

            ~CommandBufferVk() {}

            void Begin();

            void End();

            void OutputImage(SwapchainVk* swapchain);

            void InputImage();

            void Clear(ImageVk& image, ImageAspect imageAspect);

            void PrepereForPresent(ImageVk& image);

            void Reset();

            Status Wait(const TimeUnits& timeout);

        private:
            VkCommandBuffer GetHandle() { return m_commandBuffer; }

            void InsertImageMemoryBarrier(ImageVk& image,
                                     VkImageSubresourceRange& imageSubresourceRange,
                                     VkImageLayout layout);

            void ChangeState(State state) { m_state = state; }

        private:
            DeviceVk* m_device;
            State m_state;
        public: // delete me
            VkCommandBuffer m_commandBuffer;
            SemaphoreG m_semaphore;
            FenceG m_fence;

        friend DeviceVk;
    }; // class CommandBuffer

}
