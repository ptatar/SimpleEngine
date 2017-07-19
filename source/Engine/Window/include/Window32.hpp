#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "IWindow.hpp"
#include "IWindowEventHandler.hpp"

namespace engine {

class Window32 : public IWindow, public IWindowEventHandler {
public:
    Window32();
    Window32(Window32&) = delete;
    Window32(Window32&&) = delete;
    ~Window32();

    int Initialize(unsigned posX, unsigned posY, unsigned wndWidth, unsigned wndHeight) override;
    void RegisterEventCallback(IWindowEventHandler*) override;
    void Show() override;
    void Hide() override;
    bool HandleEvents() override;
    void Shutdown() override;

    WindowId GetWindowId() override;
    unsigned GetWidth() override;
    unsigned GetHeight() override;

    void OnStart() override;
    void OnResize(unsigned width, unsigned height) override;
    void OnReposition(unsigned x, unsigned y) override;
    void OnExit() override;
    bool IsFullscreen() override;

private:
    void GetWindowSize(unsigned clientWidth, unsigned clientHeight, unsigned& windowWidth, unsigned& windowHeight);
private:
    HINSTANCE m_hInstance;
    HWND      m_hWindow;
    LPCWSTR   m_windowName;
    bool      m_isFullscreen;
    unsigned  m_width;
    unsigned  m_height;

    static LRESULT CALLBACK DefaultWin32EventHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

} // namespace engine
