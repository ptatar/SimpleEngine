#pragma once

#include "IWindowEventHandler.hpp"
#include "Types.hpp"

namespace engine {


    typedef Uint64 WindowId;

    class IWindow {
    public:
        virtual ~IWindow() {};
        virtual int Initialize(unsigned posX, unsigned posY, unsigned wndWidth, unsigned wndHeight) = 0;
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void Shutdown() = 0;
        virtual void RegisterEventCallback(IWindowEventHandler*) = 0;
        virtual bool HandleEvents() = 0;
        virtual unsigned GetWidth() = 0;
        virtual unsigned GetHeight() = 0;
        virtual bool IsFullscreen() = 0;

        // TODO find a better way
        virtual WindowId GetWindowId() = 0;
    };

} // namespace engine
