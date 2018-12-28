#include "VulkanUtility.hpp"

#include "IImage.hpp"

namespace engine
{

    VkFormat ImageFormat2NativeFormat(ImageFormat format)
    {
        switch(format)
        {
            case ImageFormat::B8G8R8A8_Unorm:
                return VK_FORMAT_B8G8R8A8_UNORM;
            case ImageFormat::R8G8B8A8_UNorm:
                return VK_FORMAT_B8G8R8A8_UNORM;
            default:
                ASSERT(false);
                return VK_FORMAT_UNDEFINED;
        }
    }

    ImageFormat NativeFormat2ImageFormat(VkFormat format)
    {
        switch(format)
        {
            case VK_FORMAT_B8G8R8A8_UNORM:
                return ImageFormat::B8G8R8A8_Unorm;
            case VK_FORMAT_R8G8B8A8_UNORM:
                return ImageFormat::R8G8B8A8_UNorm;
            default:
                ASSERT(false);
                return ImageFormat::Undefined;
        }
    }

} // namespace engie
