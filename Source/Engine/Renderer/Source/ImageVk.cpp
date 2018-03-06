#include "ImageVk.hpp"

#include "DeviceVk.hpp"

namespace engine
{
    ImageVk::~ImageVk()
    {
        if (m_image != VK_NULL_HANDLE)
        {
            m_device->DestroyImage(m_image);
            m_image = VK_NULL_HANDLE;
        }

        if (m_view != VK_NULL_HANDLE)
        {
            m_device->DestroyImageView(m_view);
            m_view = VK_NULL_HANDLE;
        }
    }

    const ImageDesc& ImageVk::GetImageDesc() const
    {
        return m_imageDesc;
    }
}
