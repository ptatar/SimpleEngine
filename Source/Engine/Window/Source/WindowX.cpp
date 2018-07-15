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
        : m_display(nullptr)
        , m_window(0)
        , m_screen(0)
        , m_width(0)
        , m_height(0)
        , m_state(WindowState::NotInitialized) {}

    WindowX::~WindowX()
    {
        Finalize();
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
        m_fileDescriptor = ConnectionNumber(m_display);
        m_width = wndWidth;
        m_height = wndHeight;
        XSync(m_display, false);

        SetState(WindowState::Initialized);

        return 0;
    }

    void WindowX::RegisterEventCallback(IWindowEventHandler*) {}

    void WindowX::Show()
    {
        XMapWindow(m_display, m_window);
        XFlush(m_display);
        SetState(WindowState::Show);
        for(auto& listener: m_surfaceEventListeners)
        {
            listener->OnShow();
        }
    }

    void WindowX::Hide()
    {
        XUnmapWindow(m_display, m_window);
        XFlush(m_display);
        UnsetState(WindowState::Show);
        for(auto& listener: m_surfaceEventListeners)
        {
            listener->OnHide();
        }
    }

    Bool WindowX::HandleEvents()
    {
        fd_set fds;
        struct timeval tv;
        XEvent event;
        FD_ZERO(&fds);
        FD_SET(m_fileDescriptor, &fds);
        tv.tv_usec = 60000;
        tv.tv_sec = 0;

        Int32 numReadsFds = select(m_fileDescriptor + 1, &fds, 0, 0, &tv);
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
                        SetState(WindowState::Terminal);
                        return false;
                }
            }
        }
        if (IsShutdown()) // not sure about this
        {
            SetState(WindowState::Terminal);
            return false;
        }
        return true;
    }

    void WindowX::Finalize()
    {
        // TO check thread this should be run from main thread
        if (!CheckState(WindowState::Initialized))
        {
            return;
        }

        if (CheckState(WindowState::Show))
        {
            Hide();
        }

        Shutdown();
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
