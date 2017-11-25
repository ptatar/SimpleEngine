#pragma once

#include "XlibInclude.hpp"
#include "Types.hpp"

#include <vector>

namespace engine
{
    enum class WindowType
    {
        WindowX,
        Window32,
    };

    class IWindowSurface
    {
        public:
            virtual ~IWindowSurface() {}
            virtual ExtentI GetSurfaceExtent() const = 0;
            virtual WindowType GetWindowType() const = 0;
    };

    class IWindowSurfaceX: public IWindowSurface
    {
        public:
            virtual ~IWindowSurfaceX() {}
            virtual WindowType GetWindowType() const { return WindowType::WindowX; }
            virtual Window GetWindow() const = 0;
            virtual Display* GetDisplay() const = 0;
    };
} // namespace engine
