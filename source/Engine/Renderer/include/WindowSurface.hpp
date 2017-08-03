#pragma once

#if defined(PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
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

	};

#endif

} // namespace engine