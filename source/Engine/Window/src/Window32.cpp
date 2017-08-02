#include "Window32.hpp"
#include "Logger.hpp"

#include <strsafe.h>

namespace engine {

Window32::Window32():
    m_hInstance(0),
    m_hWindow(0),
    m_windowName(L""),
    m_isFullscreen(false),
    m_width(0),
    m_height(0) {
    };


Window32::~Window32() {};


int Window32::Initialize(Uint32 posX, Uint32 posY, Uint32 clientWidth, Uint32 clientHeight) {
    WNDCLASSEX wc;
    m_windowName = L"Window";

    m_hInstance = GetModuleHandle(nullptr);

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = DefaultWin32EventHandler;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_windowName;
    wc.cbSize = sizeof(WNDCLASSEX);

    if (!RegisterClassExW(&wc)) {
        LOGE("Window registration error!");
        return 1;
    }
    //while(true){}
    //int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    //int screenWidth = GetSystemMetrics(SM_CXSCREEN);

    DWORD style   = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_OVERLAPPEDWINDOW;
    DWORD styleEx = WS_EX_APPWINDOW;

    unsigned wndWidth, wndHeight;
    GetWindowSize(clientWidth, clientHeight, wndWidth, wndHeight);

    m_hWindow = CreateWindowEx(styleEx, m_windowName, m_windowName, style, posX, posY,
                               wndWidth, wndHeight, NULL, NULL, m_hInstance, NULL);

    if (m_hWindow == nullptr) {
        UnregisterClass(m_windowName, m_hInstance);
        return 1;
    }

    SetWindowLongPtr(m_hWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    m_width = clientWidth;
    m_height = clientHeight;

    return 0;
}


void Window32::RegisterEventCallback(IWindowEventHandler*) {
}


void Window32::Show() {
    ShowWindow(m_hWindow, SW_SHOW);
    UpdateWindow(m_hWindow);
    SetForegroundWindow(m_hWindow);
    SetFocus(m_hWindow);
}


void Window32::Hide() {
    // TODO
}


void Window32::Shutdown() {
    DestroyWindow(m_hWindow);
    m_hWindow = NULL;

    UnregisterClass(m_windowName, m_hInstance);
    m_hInstance = NULL;
}


bool Window32::HandleEvents() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT) {
        return false;
    }
    else {
        return true;
    }
}

unsigned Window32::GetWidth() {
    return m_width;
}


unsigned Window32::GetHeight() {
    return m_height;
}


void Window32::OnStart() {
        // TODO
}


void Window32::OnResize(Uint32 width, Uint32 height) {
        // TODO
}


void Window32::OnReposition(Uint32 x, Uint32 y) {

}


void Window32::OnExit() {
    DestroyWindow(m_hWindow);
}


bool Window32::IsFullscreen() {
    return m_isFullscreen;
}

Bool Window32::RequestRendererSurface()
{
	if (m_rendererManager->CreateRenderer(this);
	return true;
}


LRESULT CALLBACK Window32::DefaultWin32EventHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
    Window32* window = reinterpret_cast<Window32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (umessage) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_CLOSE: {
            if (window != nullptr) {
                window->OnExit();
            }
            break;
        }
        case WM_QUIT: {
            PostQuitMessage(0);
            break;
        }
        case WM_MOVE: {
            if (window != nullptr) {
                unsigned x = LOWORD(lparam);
                unsigned y = HIWORD(lparam);
                window->OnReposition(x, y);
            }
            break;
        }
        case WM_SIZE: {
            if (window != nullptr) {
                unsigned width = LOWORD(lparam);
                unsigned height = HIWORD(lparam);
                window->OnResize(width, height);
            }
            break;
        }
        default: {
            return DefWindowProc(hwnd, umessage, wparam, lparam);
        }
    }
    return 0;
}


void Window32::GetWindowSize(Uint32 clientWidth, Uint32 clientHeight, Uint32& windowWidth, Uint32& windowHeight) {
    // TODO
    windowWidth = clientWidth;
    windowHeight = clientHeight;
}


} // namespace engine
