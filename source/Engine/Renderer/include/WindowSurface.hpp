#pragma once

#if defined(PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(PLATFORM_LINUX)
#include <X11/Xlib.h>
#endif


namespace engine
{
#if defined(PLATFORM_WINDOWS)
	class IWindowSurface32
	{
	public:
		virtual ~IWindowSurface32() {}
		virtual HINSTANCE GetHInstance() const = 0;
		virtual HWND GetHWindow() const = 0;
	};

#elif defined(PLATFORM_LINUX)

	class IWindowSurfaceX
	{
        public:
            virtual ~IWindowSurfaceX() {}
            virtual Window GetWindow() const = 0;
            virtual Display* GetDisplay() const = 0;
	};

#endif

} // namespace engine
