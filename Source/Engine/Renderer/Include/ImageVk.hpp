#pragma once

#include "IImage.hpp"
#include "VulkanUtility.hpp"


namespace engine
{
    class DeviceVk;
    class CommandBuffer;

    class ImageVk//: public IImage
    {
        CLASS_NOT_COPYABLE(ImageVk);
        public:
            ImageVk()
                : m_device(nullptr)
                , m_image(VK_NULL_HANDLE)
                , m_view(VK_NULL_HANDLE)
                , m_layout(VK_IMAGE_LAYOUT_UNDEFINED) {}

            ImageVk(DeviceVk* device, VkImage image, VkImageView view)
                : m_image(image)
                , m_view(view)
                , m_device(device)
                , m_layout(VK_IMAGE_LAYOUT_UNDEFINED){}

            ~ImageVk();

            const ImageDesc& GetImageDesc() const;

            VkImage GetImage() const { return m_image; }

            VkImageView GetView() const { return m_view; }

            VkImageLayout GetLayout() const { return m_layout; }

        private:
            DeviceVk* m_device;

            ImageDesc m_imageDesc;

            VkImage m_image;

            VkImageView m_view;

            VkImageLayout m_layout;

            friend DeviceVk;

            friend CommandBuffer;
    };

} // namespace engine
