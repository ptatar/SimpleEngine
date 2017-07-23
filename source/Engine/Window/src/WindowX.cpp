#include "WindowX.hpp"

#include "Logger.hpp"

namespace engine
{

    WindowX::WindowX():m_display(nullptr), m_window(),m_screen(-1) {}

    Int32 WindowX::Initialize( Uint32 poxX, Uint32 posY, Uint32 wndWidth, Uint32 wndHeight)
    {
        m_display = XOpenDisplay(NULL);
        if (m_display == nullptr)
        {
            LOGE("Cound not create connection to XServer");
            return -1;
        }

        m_screen = DefaultScreen(m_display);

        Window window = XCreateSimpleWindow(m_display, DefaultRootWindow(m_display), 0, 0,
                                       800, 600, 0, BlackPixel(m_display, m_screen),
                                       WhitePixel(m_display, m_screen));

        Atom delWindow = XInternAtom( m_display, "WM_DELETE_WINDOW", 0 );
        XSetWMProtocols(m_display , m_window, &delWindow, 1);

        XSelectInput(m_display, m_window, ExposureMask);

        XMapWindow(m_display, m_window);

        return 0;
    }

    void WindowX::RegisterEventCallback(IWindowEventHandler*) {}

    void WindowX::Show() {}

    void WindowX::Hide() {}

    Bool WindowX::HandleEvents()
    {
        XEvent event;
        while(1)
        {
            XNextEvent(m_display, &event);
            if(event.type==Expose)
            {
            }
            if(event.type==ClientMessage)
                return false;
        }
        return true;
    }

    void WindowX::Shutdown() {}

    WindowId WindowX::GetWindowId() {}

    Uint32 WindowX::GetWidth() {}

    Uint32 WindowX::GetHeight() {}

    void WindowX::OnStart() {}

    void WindowX::OnResize( Uint32 width, Uint32 height) {}

    void WindowX::OnReposition( Uint32 x, Uint32 y) {}

    void WindowX::OnExit() {}

    Bool WindowX::IsFullscreen() { return false; }

} // namespace engine
