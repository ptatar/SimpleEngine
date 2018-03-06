#pragma once

#include "ISwapchain.hpp"
#include "VulkanUtility.hpp"
#include "Utility.hpp"
#include "ImageVk.hpp"

#include "Array.hpp"
#include <deque>

namespace engine
{
    class DeviceVk;

    class SwapchainVk//: public ISwapchain
    {
        public:
            SwapchainVk(): m_device(nullptr), m_swapchain(VK_NULL_HANDLE) {}

            SwapchainVk(DeviceVk* device, VkSwapchainKHR swapchain, Uint32 width, Uint32 height, Uint32 imageCount)
                : m_device(device)
                , m_swapchain(swapchain)
                , m_width(width)
                , m_height(height)
                , m_images(imageCount){}

            ~SwapchainVk();

            Status AcquireImage(TimeUnits& timeUnits);

            void PresentImage(VkQueue queue);

            Uint32 GetImageCount() const { return m_images.Size(); };

            Uint32 GetAvailableImageCount() const { return m_availableImages.size(); }

            ImageVk& GetImage(Uint32 index) { return m_images[index]; }

            ImageVk& GetCurrentImage();

            void PopImage() {m_availableImages.pop_front(); }

        private:
            VkSwapchainKHR GetSwapchain() { return m_swapchain; }

            VkSemaphore GetSemaphore() { return m_semaphore; }

            void AddImage(Uint32 imageIndex);

        private:
            DeviceVk* m_device;

            VkSwapchainKHR m_swapchain;

            VkSemaphore m_semaphore;

            std::deque<Uint32> m_availableImages;

            Array<ImageVk> m_images;

            Uint32 m_width;

            Uint32 m_height;

            friend DeviceVk;
    };
} // namespace engine
