#pragma once

#include "ICommandBuffer.hpp"
#include "VulkanUtility.hpp"

namespace engine
{

    class DeviceVk;
    class SwapchainVk;
    class ImageVk;

    class CommandBufferVk//: public ICommandBuffer
    {
        public:
            CommandBufferVk(DeviceVk* device) {}

            ~CommandBufferVk() {}

            void Begin();

            void End();

            void OutputImage(SwapchainVk* swapchain);

            void InputImage();

            void Clear(ImageVk& image, ImageAspect imageAspect);

            void Present() {}

            void Reset();

        private:
            VkCommandBuffer GetHandle() { return m_commandBuffer; }

        private:
            VkCommandBuffer m_commandBuffer;

        friend DeviceVk;
    }; // class CommandBuffer

}
