#include "Handle.hpp"

#include "DeviceVk.hpp"

namespace engine
{
#define CREATE_HANDLE_DESTRUCTOR(NAME, TYPE, DESTRUCTOR) \
    NAME::~NAME() { \
        if (m_device) \
        { \
            m_device->DESTRUCTOR(m_type); \
        } \
    }

    CREATE_HANDLE_DESTRUCTOR(SemaphoreG, VkSemaphore, DestroySemaphore);
    CREATE_HANDLE_DESTRUCTOR(SurfaceG, VkSurfaceKHR, DestroySurface);
    CREATE_HANDLE_DESTRUCTOR(CommandPoolG, VkCommandPool, DestroyCommandPool);
    CREATE_HANDLE_DESTRUCTOR(ImageViewG, VkImageView, DestroyImageView);
    CREATE_HANDLE_DESTRUCTOR(FenceG, VkFence, DestroyFence);
    CREATE_HANDLE_DESTRUCTOR(ShaderG, VkShaderModule, DestroyShader);
    CREATE_HANDLE_DESTRUCTOR(RenderPassG, VkRenderPass, DestroyRenderPass);
    CREATE_HANDLE_DESTRUCTOR(FramebufferG, VkFramebuffer, DestroyFramebuffer);
#undef CREATE_HANDLER_DESTRUCTOR

} // namespace engine
