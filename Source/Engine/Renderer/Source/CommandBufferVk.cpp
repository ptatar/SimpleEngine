#include "CommandBufferVk.hpp"

#include "SwapchainVk.hpp"
#include "DeviceVk.hpp"

namespace engine
{

    void CommandBufferVk::Begin()
    {
        ASSERT(m_state == State::Initialized || m_state == State::Submitted);
        VkCommandBufferBeginInfo info;
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.pNext = nullptr;
        info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        info.pInheritanceInfo = nullptr;

        VkResult result = vkBeginCommandBuffer(m_commandBuffer, &info);
        if (result != VK_SUCCESS)
        {
            ASSERT(false);
            return;
        }

        m_state = State::Recording;
        m_dependency.clear();
        m_dependencySet.clear();
    }

    void CommandBufferVk::OutputImage(SwapchainVk* swapchain)
    {
        /*
        VkImageSubresourceRange imageSubresourceRange = {
            VK_IMAGE_ASPECT_COLOR_BIT,
            0,
            1,
            0,
            1
        };

        VkImage renderTarget = swapchain->GetNextImageIndex();

        VkImageMemoryBarrier read2WriteBarrier;
        read2WriteBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        read2WriteBarrier.pNext = nullptr;
        read2WriteBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        read2WriteBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        read2WriteBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        read2WriteBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        read2WriteBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        read2WriteBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        read2WriteBarrier.image = renderTarget;
        read2WriteBarrier.subresourceRange = imageSubresourceRange;
        */
    }

    void CommandBufferVk::End()
    {
        ASSERT(m_state == State::Recording);
        vkEndCommandBuffer(m_commandBuffer);
        m_state = State::Ready;
    }

    void CommandBufferVk::Reset()
    {
        vkResetCommandBuffer(m_commandBuffer, 0);
        m_state = State::Initialized;
    }

    Status CommandBufferVk::Wait(const TimeUnits& timeout)
    {
        ASSERT(m_state == State::Submitted || m_state == State::Initialized);
        if (m_state == State::Initialized)
        {
            return Status::Success;
        }
        else
        {
            return m_device->WaitForFence(m_fence, timeout);
        }
    }

    void CommandBufferVk::Clear(ImageVk& image, ImageAspect imageAspect)
    {
        ASSERT(m_state == State::Recording);
        static_assert(static_cast<Uint32>(ImageAspect::Color) == VK_IMAGE_ASPECT_COLOR_BIT, "Type mismatch");
        static_assert(static_cast<Uint32>(ImageAspect::Depth) == VK_IMAGE_ASPECT_DEPTH_BIT, "Type mismatch");
        static_assert(static_cast<Uint32>(ImageAspect::Stencil) == VK_IMAGE_ASPECT_STENCIL_BIT, "Type mismatch");

        VkImageSubresourceRange imageSubresourceRange =
        {
            static_cast<Uint32>(imageAspect),
            0,
            1,
            0,
            1
        };

        if(image.GetLayout() != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            VkImageMemoryBarrier barrierToClear =
            {
                VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                nullptr,
                VK_ACCESS_MEMORY_READ_BIT,
                VK_ACCESS_TRANSFER_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                image.GetImage(),
                imageSubresourceRange
            };

            vkCmdPipelineBarrier(m_commandBuffer,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &barrierToClear);
        }

        VkClearColorValue color = { 1.0f, 0.0f, 0.0f, 1.0f };

        vkCmdClearColorImage(
                m_commandBuffer,
                image.GetImage(),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                &color,
                1,
                &imageSubresourceRange);

        AddDependency(image.GetDependency(), VK_PIPELINE_STAGE_TRANSFER_BIT);

    }

    void CommandBufferVk::PrepereForPresent(ImageVk& image)
    {
        ASSERT(m_state == State::Recording);
        VkImageSubresourceRange imageSubresourceRange =
        {
            VK_IMAGE_ASPECT_COLOR_BIT,
            0,
            1,
            0,
            1
        };

        VkImageMemoryBarrier barrierClearToPresent =
        {
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType  sType
            nullptr,                                    // const void*      pNext
            VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags    srcAccessMask
            VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags    dstAccessMask
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout    oldLayout
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout    newLayout
            VK_QUEUE_FAMILY_IGNORED,                    // uint32_t         srcQueueFamilyIndex
            VK_QUEUE_FAMILY_IGNORED,                    // uint32_t         dstQueueFamilyIndex
            image.GetImage(),                           // VkImage          image
            imageSubresourceRange
        };

        vkCmdPipelineBarrier(m_commandBuffer,
                             VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &barrierClearToPresent);

        AddDependency(image.GetDependency(), VK_PIPELINE_STAGE_TRANSFER_BIT);

    }

    void CommandBufferVk::InsertImageMemoryBarrier(ImageVk& image,
                                              VkImageSubresourceRange& imageSubresourceRange,
                                              VkImageLayout layout)
    {
        //vkCmdPipelineBarrier(m_commandBuffer, srcLayout,
    }

    void CommandBufferVk::AddDependency(const SemaphoreH& semaphore, VkPipelineStageFlags pipelineStageFlag)
    {
        // TODO little overkill but will do for now
        const auto ret = m_dependencySet.insert(semaphore);
        if (ret.second)
        {
            m_dependency.push_back(semaphore);
            m_dependencyStage.push_back(pipelineStageFlag);
        }
    }

} //namespace engine
