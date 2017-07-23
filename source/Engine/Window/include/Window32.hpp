#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "IWindow.hpp"
#include "IWindowEventHandler.hpp"
#include "Renderer.hpp"

namespace engine {

class Window32 : public IWindow, public IWindowEventHandler {
public:
    Window32();
    Window32(Window32&) = delete;
    Window32(Window32&&) = delete;
    ~Window32();

    Int32 Initialize(Uint32 posX, Uint32 posY, Uint32 wndWidth, Uint32 wndHeight) override;
    void RegisterEventCallback(IWindowEventHandler*) override;
    void Show() override;
    void Hide() override;
    Bool HandleEvents() override;
    void Shutdown() override;

    WindowId GetWindowId() override;
    Uint32 GetWidth() override;
    Uint32 GetHeight() override;

    void OnStart() override;
    void OnResize(Uint32 width, Uint32 height) override;
    void OnReposition(Uint32 x, Uint32 y) override;
    void OnExit() override;
    Bool IsFullscreen() override;

private:
    void GetWindowSize(Uint32 clientWidth, Uint32 clientHeight, Uint32& windowWidth, Uint32& windowHeight);
    Bool RequestRendererSurface(Renderer* renderer);
private:
    HINSTANCE m_hInstance;
    HWND      m_hWindow;
    LPCWSTR   m_windowName;
    Bool      m_isFullscreen;
    Uint32  m_width;
    Uint32  m_height;

    static LRESULT CALLBACK DefaultWin32EventHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

} // namespace engine
