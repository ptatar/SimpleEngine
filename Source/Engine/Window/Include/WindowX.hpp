#pragma once

#include "IWindow.hpp"
#include "IWindowEventHandler.hpp"
#include "WindowSurface.hpp"
#include "IRenderer.hpp"
#include "Types.hpp"
#include "Logger.hpp"

#include <memory>

namespace engine {

    class WindowX: public IWindow, public IWindowEventHandler, public IWindowSurfaceX, private Referenceable
    {
    public:
        WindowX();
        WindowX(WindowX&) = delete;
        WindowX(WindowX&&) = delete;
        virtual ~WindowX() {};

        Int32 Initialize(Uint32 poxX, Uint32 posY, Uint32 wndWidth, Uint32 wndHeight) override;
        void RegisterEventCallback(IWindowEventHandler*) override;
        void Show() override;
        void Hide() override;
        Bool HandleEvents() override;
        void Shutdown() override;

        Uint32 GetWidth() override { return m_width; }
        Uint32 GetHeight() override { return m_height; }

        void OnStart() override;
        void OnResize(Uint32 width, Uint32 height) override;
        void OnReposition( Uint32 x, Uint32 y) override;
        void OnExit() override;
        Bool IsFullscreen() override;

        //IWindowSurfaceX
        virtual Window GetWindow() const override { return m_window; }
        virtual Display* GetDisplay() const override { return m_display; }
        virtual ExtentI GetSurfaceExtent() const override { return ExtentI(m_width, m_height); }
        Bool BindRenderer(std::unique_ptr<IRenderer> renderer);

    private:
        Bool     m_initialized;
        Display* m_display;
        Window   m_window;
        Int32    m_screen;
        Uint32   m_width;
        Uint32   m_height;
        std::unique_ptr<IRenderer> m_renderer;
    }; // class WindowX

} // namespace engine
