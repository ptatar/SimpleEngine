#pragma once

#include "IWindow.hpp"
#include "IWindowEventHandler.hpp"
#include "Types.hpp"
#include "Logger.hpp"

namespace engine {

class WindowX: public IWindow, public IWindowEventHandler {
public:
    WindowX();
    WindowX(WindowX&) = delete;
    WindowX(WindowX&&) = delete;
    ~WindowX();

    int Initialize(Uint32 poxX, Uint32 posY, Uint32 wndWidth, Uint32 wndHeight) override;
};

} // namespace engine
