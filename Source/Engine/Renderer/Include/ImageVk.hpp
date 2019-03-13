#pragma once

#include "IImage.hpp"
#include "VulkanUtility.hpp"
#include "Handle.hpp"

#include <vector>

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

            ImageVk(DeviceVk* device, VkImage image, VkImageView view, const ImageDesc& imageDesc)
                : m_image(image)
                , m_view(view)
                , m_device(device)
                , m_layout(VK_IMAGE_LAYOUT_UNDEFINED)
                , m_imageDesc(imageDesc){}

            ~ImageVk();

            const ImageDesc& GetImageDesc() const;

            VkImage GetImage() const { return m_image; }

            VkImageView GetView() const { return m_view; }

            VkImageLayout GetLayout() const { return m_layout; }

            void ChangeLayout(VkImageLayout layout);

            inline void AddDependency(const SemaphoreH& semaphore) { m_dependency = semaphore; }

            inline SemaphoreH& GetDependency() { return m_dependency; }

        private:
            DeviceVk* m_device;

            ImageDesc m_imageDesc;

            VkImage m_image;

            VkImageView m_view;

            VkImageLayout m_layout;

            SemaphoreH m_dependency;

            friend DeviceVk;

            friend CommandBuffer;
    };

    class ImageVkArray
    {
        CLASS_NOT_COPYABLE(ImageVkArray);
        public:
            ImageVkArray()
                : m_device(nullptr)
                , m_images(nullptr)
                , m_views(nullptr)
                , m_layouts(nullptr) {}

            ImageViewArray(DeviceVk* device,
                           const std::vector<ImageDesc>& desc,
                           std::vector<VkImage>&& images,
                           std::vector<VkImageView>&& views)
            {}

        private:
            DeviceVk* m_device;
            std::vector<ImageDesc> m_imageDesc;
            std::vector<VkImage> m_images;
            std::vector<VkImageView> m_imageView;
    };

} // namespace engine
