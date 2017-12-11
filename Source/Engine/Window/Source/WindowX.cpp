#include "WindowX.hpp"

#include "Logger.hpp"
#include "Utility.hpp"
#include "IJob.hpp"

namespace engine
{
    enum Events
    {
        WM_SHUTDOWN,
        WM_WAKE_UP
    };

    WindowX::WindowX()
        : m_initialized(false)
        , m_display(nullptr)
        , m_window(0)
        , m_screen(0)
        , m_width(0)
        , m_height(0)
        , m_shutdown(false)
        , m_show(false) {}

    WindowX::~WindowX()
    {
        Shutdown();
    }

    Int32 WindowX::Initialize( Uint32 poxX, Uint32 posY, Uint32 wndWidth, Uint32 wndHeight)
    {
        m_display = XOpenDisplay(NULL);
        if (m_display == nullptr)
        {
            LOGE("Cound not create connection to XServer");
            return -1;
        }

        m_screen = DefaultScreen(m_display);

        m_window = XCreateSimpleWindow(m_display, DefaultRootWindow(m_display), 0, 0,
                                       800, 600, 0, BlackPixel(m_display, m_screen),
                                       WhitePixel(m_display, m_screen));
        Atom m_shutdownAtom = XInternAtom( m_display, "WM_DELETE_WINDOW", 0 );

        XSetWMProtocols(m_display , m_window, &m_shutdownAtom, 1);

        XSelectInput(m_display, m_window, ExposureMask);
        m_width = wndWidth;
        m_height = wndHeight;
        XSync(m_display, false);
        m_initialized = true;

        return 0;
    }

    void WindowX::RegisterEventCallback(IWindowEventHandler*) {}

    void WindowX::Show()
    {
        XMapWindow(m_display, m_window);
        XFlush(m_display);
        m_show = true;
        for(auto& listener: m_surfaceEventListeners)
        {
            listener->OnShow();
        }
    }

    void WindowX::Hide()
    {
        XUnmapWindow(m_display, m_window);
        XFlush(m_display);
        m_show = false;
        for(auto& listener: m_surfaceEventListeners)
        {
            listener->OnHide();
        }
    }

    Bool WindowX::HandleEvents()
    {
        Int32 fd = ConnectionNumber(m_display);
        fd_set fds;
        struct timeval tv;

        XEvent event;
        while(1)
        {
            FD_ZERO(&fds);
            FD_SET(fd, &fds);
            tv.tv_usec = 50000;
            tv.tv_sec = 0;

            Int32 numReadsFds = select(fd+1, &fds, 0, 0, &tv);
            if (numReadsFds)
            {
                while(XPending(m_display))
                {
                    XNextEvent(m_display, &event);
                    switch(event.type)
                    {
                        case Expose:
                            break;
                        case ClientMessage:
                            return false;
                    }
                }
            }
            if (m_shutdown)
            {
                return false;
            }
        }
        return true;
    }

    void WindowX::Shutdown()
    {
        // TO check thread this should be run from main thread
        if (!m_initialized)
        {
            return;
        }

        OnShutdown();
        if (m_show)
        {
            Hide();
        }

        m_shutdown = true;
        while(IsRunning())
        {
            Sleep(TimeUnits::MakeMiliseconds(200));
        }

        XDestroyWindow(m_display, m_window);
        XCloseDisplay(m_display);
        m_initialized = false;
    }

    void WindowX::OnStart() {}

    void WindowX::OnResize( Uint32 width, Uint32 height)
    {
        for (auto& listener: m_surfaceEventListeners)
        {
            listener->OnResize(width, height);
        }
    }

    void WindowX::OnReposition( Uint32 x, Uint32 y)
    {
        for (auto& listener: m_surfaceEventListeners)
        {
            listener->OnReposition(x, y);
        }
    }

    void WindowX::OnShutdown()
    {
        for (auto& listener: m_surfaceEventListeners)
        {
            listener->OnShutdown();
        }
    }

    void WindowX::OnFullscreen()
    {
        for (auto& listener: m_surfaceEventListeners)
        {
            listener->OnFullscreen();
        }
    }

    Bool WindowX::IsFullscreen() { return false; }

    void WindowX::RegisterEventListener(ObjectRef<ISurfaceEventListener>& surfaceEventListener)
    {
        m_surfaceEventListeners.push_back(surfaceEventListener);
    }

    void WindowX::UnregisterEventListener(ObjectRef<ISurfaceEventListener>& surfaceEventListener)
    {
        m_surfaceEventListeners.remove(surfaceEventListener);
    }

} // namespace engine
