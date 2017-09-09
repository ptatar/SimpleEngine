#pragma once

#if defined(PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#elif defined (PLATFORM_LINUX)
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/Xos.h>

    #undef Bool
    #undef Status
    #undef Success
#endif
