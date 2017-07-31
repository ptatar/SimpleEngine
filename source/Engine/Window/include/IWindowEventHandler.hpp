#pragma once

#include "Types.hpp"

namespace engine {

class IWindowEventHandler {
public:
    virtual ~IWindowEventHandler() {};

    virtual void OnStart() = 0;
    virtual void OnExit() = 0;
    virtual void OnResize(Uint32 width, Uint32 height) = 0;
    virtual void OnReposition(Uint32 x, Uint32 y) = 0;
};

} // namespace engine
