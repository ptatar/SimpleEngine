#pragma once

#include "IWindow.hpp"
#include "IWindowEventHandler.hpp"
#include "IRenderer.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>

namespace engine {

class Window32 : public IWindow, public IWindowEventHandler, public IWindowSurface32 {
public:
    Window32();
    Window32(Window32&) = delete;
    Window32(Window32&&) = delete;
    virtual ~Window32();

    Int32 Initialize(Uint32 posX, Uint32 posY, Uint32 wndWidth, Uint32 wndHeight) override;
    void RegisterEventCallback(IWindowEventHandler*) override;
    void Show() override;
    void Hide() override;
    Bool HandleEvents() override;
    void Shutdown() override;

    Uint32 GetWidth() override;
    Uint32 GetHeight() override;

    void OnStart() override;
    void OnResize(Uint32 width, Uint32 height) override;
    void OnReposition(Uint32 x, Uint32 y) override;
    void OnExit() override;
    Bool IsFullscreen() override;

    virtual HINSTANCE GetHInstance() const override { return m_hInstance; }
    virtual HWND GetHWindow() const override { return m_hWindow; }
    virtual ExtentI GetSurfaceExtent() const { return ExtentI(m_width, m_height); }
    Bool BindRenderer(std::unique_ptr<IRenderer> renderer); // FIXME when this function fails unique ptr is still consumed
private:
    Bool GetWindowSize(Uint32 clientWidth, Uint32 clientHeight, Uint32& windowWidth, Uint32& windowHeight, Uint32 style, Uint32 styleEx);
private:
    HINSTANCE m_hInstance;
    HWND      m_hWindow;
    LPCWSTR   m_windowName;
    Bool      m_isFullscreen;
    Uint32    m_width;
    Uint32    m_height;
    std::unique_ptr<IRenderer> m_renderer;

    static LRESULT CALLBACK DefaultWin32EventHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

} // namespace engine
