#pragma once

#include "IWindowEventHandler.hpp"
#include "Types.hpp"
#include "IJob.hpp"
#include "WindowSurface.hpp"

namespace engine
{

    class IRenderer;

    enum WindowState
    {
        NotInitialized = 0,
        Initialized    = 1 << 1,
        Show           = 1 << 2,
        Hide           = 1 << 3,
        Fullscreen     = 1 << 4,
        Terminal       = 1 << 5,
    };

    class IWindow
    {
    public:
        virtual ~IWindow() {};

        virtual void Show() = 0;

        virtual void Hide() = 0;

        virtual void Finalize() = 0;

        virtual Uint32 GetWidth() = 0;

        virtual Uint32 GetHeight() = 0;

        virtual Bool IsFullscreen() = 0;

        virtual IWindowSurface* GetSurface() = 0;

        virtual WindowState GetState() const = 0;

        virtual Bool CheckState(WindowState) const = 0;

    };

} // namespace engine
