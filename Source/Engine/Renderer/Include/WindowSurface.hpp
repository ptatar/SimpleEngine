#pragma once

#include "XlibInclude.hpp"
#include "Types.hpp"
#include "IJob.hpp"
#include <vector>

namespace engine
{

    enum class WindowType
    {
        WindowX,
        Window32,
    };

    class ISurfaceEventListener
    {
        public:
            ~ISurfaceEventListener() {}
            virtual void OnResize(Uint32 width, Uint32 height) = 0;
            virtual void OnReposition(Uint32 x, Uint32 y) = 0;
            virtual void OnShow() = 0;
            virtual void OnHide() = 0;
            virtual void OnFullscreen() = 0;
            virtual void OnShutdown() = 0;
    };

    class IWindowSurface
    {
        public:
            virtual ~IWindowSurface() {}
            virtual ExtentI GetSurfaceExtent() const = 0;
            virtual WindowType GetWindowType() const = 0;
            virtual void RegisterEventListener(ObjectRef<ISurfaceEventListener>& surfaceEventListener) = 0;
            virtual void UnregisterEventListener(ObjectRef<ISurfaceEventListener>& surfaceEventListener) = 0;
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
