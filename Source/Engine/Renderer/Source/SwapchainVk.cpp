#include "SwapchainVk.hpp"

#include "DeviceVk.hpp"

namespace engine
{
    SwapchainVk::~SwapchainVk()
    {
        if (m_device)
        {
            m_device->DestroySwapchain(m_swapchain);
        }
    }

    Status SwapchainVk::AcquireImage(TimeUnits& timeout)
    {
        return m_device->AcquireSwapchainImage(*this, timeout);
    }

    void SwapchainVk::PresentImage(VkQueue queue)
    {
        VkResult result;
        VkPresentInfoKHR info;
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.pNext = nullptr;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &m_semaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &m_swapchain;
        info.pImageIndices = &m_availableImages.front();
        info.pResults = &result;

        vkQueuePresentKHR(queue, &info);
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
