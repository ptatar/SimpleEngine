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
                return VK_FORMAT_UNDEFINED;
        }
    }

} // namespace engie
