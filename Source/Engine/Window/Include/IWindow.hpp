#pragma once

#include "IWindowEventHandler.hpp"
#include "Types.hpp"
#include "IJob.hpp"
#include "WindowSurface.hpp"

namespace engine
{

    class IRenderer;

    class IWindow
    {
    public:
        virtual ~IWindow() {};
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void Shutdown() = 0;
        virtual Uint32 GetWidth() = 0;
        virtual Uint32 GetHeight() = 0;
        virtual Bool IsFullscreen() = 0;
        virtual IWindowSurface* GetSurface() = 0;
    };

} // namespace engine
