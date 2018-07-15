#include "SwapchainVk.hpp"

#include "DeviceVk.hpp"
#include "CommandBufferVk.hpp"

namespace engine
{
    SwapchainVk::~SwapchainVk()
    {
        if (m_device)
        {
            m_device->DestroySwapchain(m_swapchain);
        }
    }

    Status SwapchainVk::AcquireImage()
    {
        return m_device->AcquireSwapchainImage(*this);
    }

    void SwapchainVk::PresentImage(ObjectRef<CommandBufferVk>& commandBuffer)
    {
        ASSERT(m_availableImages.size());

        Uint32 imageIndex = m_availableImages.front();
        m_availableImages.pop_front();

        VkResult result;
        VkPresentInfoKHR info;
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.pNext = nullptr;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &commandBuffer->m_semaphore.Get();
        info.swapchainCount = 1;
        info.pSwapchains = &m_swapchain;
        info.pImageIndices = &imageIndex;
        info.pResults = &result;

        result = vkQueuePresentKHR(m_device->GetPresentQueue(), &info);
        ASSERT(result == VK_SUCCESS);
    }

    void SwapchainVk::SyncPresentImage(ObjectRef<CommandBufferVk>& commandBuffer)
    {
    }

    ImageVk& SwapchainVk::GetCurrentImage()
    {
        return m_images[m_availableImages.front()];
    }

    void SwapchainVk::AddImage(Uint32 imageIndex)
    {
        m_availableImages.push_back(imageIndex);
    }

} // namespace engine
