#pragma once

#include "VulkanUtility.hpp"

namespace engine
{
    class DeviceVk;

    // I don't think it was even worth to write this thing
    // TODO remove it or smth
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
    CREATE_GUARD(ShaderG, VkShaderModule, DestroyShader);
    CREATE_GUARD(RenderPassG, VkRenderPass, DestroyRenderPass);
    CREATE_GUARD(FramebufferG, VkFramebuffer, DestroyFramebuffer);

#undef CREATE_GUARD

#ifdef DEBUG

    #define CREATE_HANDLE(NAME, GUARD_TYPE, HANDLE_TYPE) \
        class NAME \
        { \
            public: \
            NAME(): m_handle(VK_NULL_HANDLE) {} \
            NAME(const NAME& other): m_handle(other.m_handle) {} \
            NAME(GUARD_TYPE& guard): m_handle(guard.Get()) {} \
            operator HANDLE_TYPE() const \
            { \
                return m_handle; \
            } \
            bool operator <(const NAME& other) const \
            { \
                return m_handle < other.m_handle; \
            } \
            bool operator >(const NAME& other) const \
            { \
                return m_handle > other.m_handle; \
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
            NAME(const NAME& other): m_handle(other.m_handle) {} \
            NAME(GUARD_TYPE& guard): m_handle(guard.Get()) {} \
            operator HANDLE_TYPE() const \
            { \
                return m_handle; \
            } \
            bool operator <(const NAME& other) const \
            { \
                return m_handle < other.m_handle; \
            } \
            bool operator >(const NAME& other) const \
            { \
                return m_handle > other.m_handle; \
            } \
            private: \
                HANDLE_TYPE m_handle; \
        }; \
        static_assert(sizeof(NAME) == sizeof(HANDLE_TYPE), ""#NAME" different size than "#HANDLE_TYPE);
#endif

    CREATE_HANDLE(SemaphoreH, SemaphoreG, VkSemaphore);
    CREATE_HANDLE(SurfaceH, SurfaceG, VkSurfaceKHR);
    CREATE_HANDLE(CommandPoolH, CommandPoolG, VkCommandPool);
    CREATE_HANDLE(ImageViewH, ImageViewG, VkImageView);
    CREATE_HANDLE(FenceH, FenceG, VkFence);
    CREATE_HANDLE(ShaderH, ShaderG, VkShaderModule);
    CREATE_HANDLE(RenderPassH, RenderPassG, VkRenderPass);
    CREATE_HANDLE(FramebufferH, FramebufferG, VkFramebuffer);


#undef CREATE_HANDLE

} // namespace engine
