#pragma once

#include "Types.hpp"

namespace engine
{
    class ISwapchain;
    class IImage;

    enum class ImageAspect: Uint32
    {
        Color   = 1 << 0, //= VK_IMAGE_ASPECT_COLOR_BIT,
        Depth   = 1 << 1, //= VK_IMAGE_ASPECT_DEPTH_BIT,
        Stencil = 1 << 2, //= VK_IMAGE_ASPECT_STENCIL_BIT,
    };

    class ICommandBuffer
    {
        public:
            virtual ~ICommandBuffer() {}

            virtual void Begin() = 0;

            virtual void End() = 0;

            virtual void OutputImage(ISwapchain* swapchain) = 0;

            virtual void InputImage() = 0;

            virtual void Clear(IImage& image, ImageAspect aspect) = 0;

            virtual void Present() = 0;

            virtual void Reset() = 0;
    };

} // namespace engine
