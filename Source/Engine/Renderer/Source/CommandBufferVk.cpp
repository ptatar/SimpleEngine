#include "CommandBufferVk.hpp"

#include "SwapchainVk.hpp"

namespace engine
{

    void CommandBufferVk::Begin()
    {
        /*
        VkImageMemoryBarrier write2ReadBarrier;
        write2ReadBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        write2ReadBarrier.pNext = nullptr;
        write2ReadBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        write2ReadBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        write2ReadBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        write2ReadBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        write2ReadBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        write2ReadBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        write2ReadBarrier.image = renderTarget;
        write2ReadBarrier.subresourceRange = imageSubresourceRange;

        VkCommandBufferBeginInfo info;
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.pNext = nullptr;
        info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        info.pInheritanceInfo = nullptr;

        VkResult result = vkBeginCommandBuffer(m_commandBuffer, &info);
        if (result == VK_SUCCESS)
        {
            ASSERT(false);
            return;
        }

        vkCmdPipelineBarrier(
                m_commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0,
                nullptr,
                0,
                nullptr,
                1,
                &read2WriteBarrier);



        vkCmdPipelineBarrier(
                m_commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                0,
                0,
                nullptr,
                0,
                nullptr,
                1,
                &write2ReadBarrier);
                */
        VkCommandBufferBeginInfo info;
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.pNext = nullptr;
        info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        info.pInheritanceInfo = nullptr;
    }

    void CommandBufferVk::OutputImage(Swapchain* swapchain)
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
        vkEndCommandBuffer(m_commandBuffer);
    }

    void CommandBufferVk::Reset()
    {
        vkResetCommandBuffer(m_commandBuffer, 0);
    }

    void CommandBufferVk::Clear(Image& image, ImageAspect imageAspect)
    {
        static_assert(static_cast<Uint32>(ImageAspect::Color) == VK_IMAGE_ASPECT_COLOR_BIT, "Type mismatch");
        static_assert(static_cast<Uint32>(ImageAspect::Depth) == VK_IMAGE_ASPECT_DEPTH_BIT, "Type mismatch");
        static_assert(static_cast<Uint32>(ImageAspect::Stencil) == VK_IMAGE_ASPECT_STENCIL_BIT, "Type mismatch");

        VkImageSubresourceRange imageSubresourceRange = {
            static_cast<Uint32>(imageAspect),
            0,
            1,
            0,
            1
        };

        VkClearColorValue color = { 1.0f, 0.0f, 0.0f, 1.0f };

        vkCmdClearColorImage(
                m_commandBuffer,
                image.GetImage(),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                &color,
                1,
                &imageSubresourceRange);
        //vkCmdClearColorImage(m_commandBuffer,
    }

} //namespace engine
