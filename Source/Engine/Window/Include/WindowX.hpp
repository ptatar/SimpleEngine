#pragma once

#include "IWindow.hpp"
#include "IWindowEventHandler.hpp"
#include "WindowSurface.hpp"
#include "IRenderer.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "IJob.hpp"
#include "RendererManager.hpp"

#include <memory>

namespace engine
{
    class WindowX: public IWindow, public IWindowSurfaceX, public IJob
    {
    public:
        WindowX();
        WindowX(WindowX&) = delete;
        WindowX(WindowX&&) = delete;
        virtual ~WindowX() {};
        Int32 Initialize(Uint32 poxX, Uint32 posY, Uint32 wndWidth, Uint32 wndHeight);
        void RegisterEventCallback(IWindowEventHandler*);
        void Show() override;
        void Hide() override;
        Bool HandleEvents();;
        void Shutdown() override;

        Uint32 GetWidth() override { return m_width; }
        Uint32 GetHeight() override { return m_height; }

        void OnStart();
        void OnResize(Uint32 width, Uint32 height);
        void OnReposition( Uint32 x, Uint32 y);
        void OnExit();
        Bool IsFullscreen() override;

        //IWindowSurface
        virtual Window GetWindow() const override { return m_window; }
        virtual Display* GetDisplay() const override { return m_display; }
        virtual ExtentI GetSurfaceExtent() const override { return ExtentI(m_width, m_height); }
        virtual IWindowSurfaceX* GetSurface() override { return this; }
        Bool Work() override { return HandleEvents(); }

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
