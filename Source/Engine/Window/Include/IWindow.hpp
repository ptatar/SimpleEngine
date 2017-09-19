#pragma once

#include "IWindowEventHandler.hpp"
#include "Types.hpp"

namespace engine {


    typedef Uint64 WindowId;

    class IWindow: private Referenceable {
    public:
        virtual ~IWindow() {};
        virtual int Initialize(Uint32 posX, Uint32 posY, Uint32 wndWidth, Uint32 wndHeight) = 0;
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void Shutdown() = 0;
        virtual void RegisterEventCallback(IWindowEventHandler*) = 0;
        virtual Bool HandleEvents() = 0;
        virtual Uint32 GetWidth() = 0;
        virtual Uint32 GetHeight() = 0;
        virtual Bool IsFullscreen() = 0;

    };

} // namespace engine
