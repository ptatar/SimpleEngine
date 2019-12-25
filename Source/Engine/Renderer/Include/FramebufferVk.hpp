#pragma once

#include "ImageVk.hpp"

namespace engine
{
    class DeviceVk;

    class FramebufferDesc
    {
        public:
            FramebufferDesc() {}

            ~FramebufferDesc() {}

            void AddRenderTarget(const ObjectRef<ImageVk>& renderTarget);

            void AddDepthTarget(const ObjectRef<ImageVk>& depthTarget);

            const std::vector<ObjectRef<ImageVk>>& GetRenderTargets() const { return m_renderTargets; }

            const ObjectRef<ImageVk>& GetDepthTarget() const { return m_depthTarget; }

            Uint32 GetWidth() const { return m_width; }

            Uint32 GetHeight() const { return m_height; }

        private:
            std::vector<ObjectRef<ImageVk>> m_renderTargets;
            ObjectRef<ImageVk> m_depthTarget;
            Uint32 m_width;
            Uint32 m_height;
    };

    class FramebufferVk
    {
        public:
            FramebufferVk(DeviceVk* device, VkFramebuffer framebuffer)
                : m_device(device)
                , m_framebuffer(framebuffer) {}

            FramebufferVk(std::vector<ObjectRef<ImageVk>>& renderTargets);

            FramebufferVk(ObjectRef<ImageVk>& renderTarget);

            ~FramebufferVk();

            const std::vector<VkImage> GetRenderTargetImages() const { return m_renderTargetImages; }

            const std::vector<VkImageView> GetRenderTargetViews() const { return m_renderTargetViews; }

            const std::vector<ObjectRef<ImageVk>> GetImages() const { return m_imageRef;}

        private:
            std::vector<ObjectRef<ImageVk>> m_imageRef;

            std::vector<VkImage> m_renderTargetImages;

            std::vector<VkImageView> m_renderTargetViews;

            VkImage m_depthImage;

            VkImageView m_depthView;

            VkFramebuffer m_framebuffer;

            DeviceVk* m_device;

            friend DeviceVk;
    };
}
