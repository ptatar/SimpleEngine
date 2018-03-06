#pragma once

#include "Types.hpp"

namespace engine
{
    struct ImageDesc
    {
        Uint32 width;
        Uint32 height;
    };

    class IImage
    {
        public:
            virtual ~IImage() {}

            virtual const ImageDesc& GetImageDesc() const = 0;

    };

} // namespace engine
