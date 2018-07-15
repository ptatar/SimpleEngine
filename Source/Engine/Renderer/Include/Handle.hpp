#pragma once

#include "VulkanUtility.hpp"

namespace engine
{
    class DeviceVk;

    // I don't think it was even worth to write this thing
#define CREATE_GUARD(NAME, TYPE, DESTRUCTOR) \
    class NAME \
    { \
    public: \
        NAME(): m_device(nullptr), m_type(VK_NULL_HANDLE) {} \
        NAME(DeviceVk* device, TYPE type): m_device(device), m_type(type) {} \
        NAME(const NAME&) = delete; \
        NAME(NAME&& rs) \
        { \
            m_device = rs.m_device; \
            m_type = rs.m_type; \
            rs.m_device = nullptr; \
        } \
        ~NAME(); \
        NAME& operator=(const NAME&) = delete; \
        NAME& operator=(NAME&& rs) \
        { \
          m_device = rs.m_device; \
          rs.m_device = nullptr; \
          m_type = rs.m_type; \
          return *this; \
        } \
        inline TYPE& Get() \
        { \
            return m_type; \
        } \
        operator Bool() \
        { \
            return m_type != VK_NULL_HANDLE;\
        } \
    private: \
        TYPE m_type; \
        DeviceVk* m_device; \
    };

    CREATE_GUARD(SemaphoreG, VkSemaphore, DestroySemaphore);
    CREATE_GUARD(SurfaceG, VkSurfaceKHR, DestroySurface);
    CREATE_GUARD(CommandPoolG, VkCommandPool, DestroyCommandPool);
    CREATE_GUARD(ImageViewG, VkImageView, DestroyImageView);
    CREATE_GUARD(FenceG, VkFence, DestroyFence);


#undef CREATE_GUARD

#ifdef DEBUG

    #define CREATE_HANDLE(NAME, GUARD_TYPE, HANDLE_TYPE) \
        class NAME \
        { \
            public: \
            NAME(): m_handle(VK_NULL_HANDLE) {} \
            NAME(GUARD_TYPE& guard): m_handle(guard.Get()) {} \
            operator HANDLE_TYPE() \
            { \
                return m_handle; \
            } \
            private: \
                HANDLE_TYPE m_handle; \
        };

#else

    #define CREATE_HANDLE(NAME, GUARD_TYPE, HANDLE_TYPE) \
        class NAME \
        { \
            public: \
            NAME(): m_handle(VK_NULL_HANDLE) {} \
            NAME(GUARD_TYPE& guard): m_handle(guard.Get()) {} \
            operator HANDLE_TYPE() \
            { \
                return m_handle; \
            } \
            private: \
                HANDLE_TYPE m_handle; \
        };

#endif

    CREATE_HANDLE(SemaphoreH, SemaphoreG, VkSemaphore);
    CREATE_HANDLE(SurfaceH, SurfaceG, VkSurfaceKHR);
    CREATE_HANDLE(CommandPoolH, CommandPoolG, VkCommandPool);
    CREATE_HANDLE(ImageViewH, ImageViewG, VkImageView);
    CREATE_HANDLE(FenceH, FenceG, VkFence);

#undef CREATE_HANDLE

} // namespace engine
