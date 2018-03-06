#pragma once

#include "IImage.hpp"
#include "Utility.hpp"

namespace engine
{

    class ISwapchain
    {
        public:
            virtual ~ISwapchain() {}

            virtual Status AcquireImage(TimeUnits& timeUnits) = 0;

            virtual Uint32 GetImageCount() const = 0;

            virtual Uint32 GetAvailableImageCount() const = 0;

            virtual IImage& GetImage(Uint32) = 0;

            virtual IImage& GetCurrentImage() = 0;

            virtual void PopImage() = 0;
    };

} // namespace engine
