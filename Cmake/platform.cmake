include(FindVulkan)
if(NOT ${Vulkan_FOUND})
    message(FATAL_ERROR "Vulkan not found")
endif()

if(WIN32)
    set(PLATFORM_WINDOWS TRUE)
    add_definitions(-D_UNICODE
                    -DPLATFORM_WINDOWS
                    -DWINVER=_WIN32_WINNT_WIN7
                    -D_WIN32_WINNT=_WIN32_WINNT_WIN7)
elseif(UNIX)
    set(PLATFORM_LINUX TRUE)
    add_definitions(-DPLATFORM_LINUX)
    include(FindX11)
    if(NOT ${X11_FOUND})
        message(FATAL_ERROR "X11 not found")
    endif()

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
else()
    message(FATAL_ERROR "Unknown platform")
endif()
