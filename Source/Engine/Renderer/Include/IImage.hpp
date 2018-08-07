#pragma once

#include "Types.hpp"

namespace engine
{
    enum class ImageFormat
    {
        R8G8B8A8_UNorm,
        B8G8R8A8_Unorm,
        Undefined,
    };

    struct ImageDesc
    {
        ImageDesc()
            : width(0)
            , height(0)
            , format(ImageFormat::Undefined) {}

        ImageDesc(Uint32 _width, Uint32 _height, ImageFormat _format)
            : width(_width)
            , height(_height)
            , format(_format) {}

        Uint32      width;
        Uint32      height;
        ImageFormat format;
    };

    class IImage
    {
        public:
            virtual ~IImage() {}

            virtual const ImageDesc& GetImageDesc() const = 0;

    };

} // namespace engine
